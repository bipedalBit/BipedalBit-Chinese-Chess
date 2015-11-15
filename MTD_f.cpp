#include "MTD_f.h"
#include "SerialGenerator.h"
#include <ctime>
#include <functional>
#include <bitset>

class State;

std::size_t KeyHash::operator()(const State &s) const
{
	std::bitset<8> tmp;
	std::string str = "";
	for (int i = 0; i < 32; ++i)
	{
		tmp = s.posList[i].pos;
		str += tmp.to_string();
	}
	/* 局面相同，主动权归属不同，并不是同一个局面状态 */
	str += (s.RTurn ? "1" : "0");
	/* 32*8 + 1 = 257 bits */
	std::bitset<0x101> state(str);
	return std::hash<std::bitset<0x101>>()(state);
}

MTD_f::MTD_f(unsigned char _maxDepth)
{
	/* 初始化maxDepth值 */
	maxDepth = _maxDepth;
	/* 预设置换表大小，减少内存申请开销 */
	/* 理论上来说应该按照硬件条件使hash表尽量大 */
	/* 但是hash表越大越有申请失败的危险，姑且按照经验设定一个合适的值 */
	TT.reserve(HASH_SIZE);
}

void MTD_f::search(State &s, std::vector<State> &record)
{
	/* 每次搜索前评估结点数清零 */
	evaluator->cnt = 0;
	/* 在搜索核心类对象中记录局面状态，以免反复传参 */
	currentState = s;
	/* 复制局面记录指针 */
	stateRecord = record;
	/* 如果还没有搜索过，使用当前局面的评分作为第一次评分的猜测值 */
	int firstGuess = evaluator->evaluate(s);
	/* 开始计时 */
	clock_t t0 = clock();
	clock_t t1;
	/* 迭代深化，有hash置换表加速，但是迭代深化会增加重复搜索的开销，尝试逐步加深搜索深度 */
	for (tryDepth = 1; tryDepth <= maxDepth; ++tryDepth)
	{
		/* 进行MTD(f)搜索，更新首次猜测评分 */
		firstGuess = mtdf(firstGuess);
		/* 掐表检查时间 */
		t1 = clock() - t0;
		/* 如果搜索时间大于1秒，停止迭代深化搜索 */
		/* searchDepth = min{timeoutDepth, maxDepth} */
		if ((double)t1/CLOCKS_PER_SEC > 1)
		{
			/* 记录搜索用时 */
			searchTime = (double)t1/CLOCKS_PER_SEC;
			/* 记录搜索局面数 */
			searchCnt = evaluator->cnt;
			return;
		}
	}
	/* 记录搜索用时 */
	searchTime = (double)t1/CLOCKS_PER_SEC;
	/* 记录搜索局面数 */
	searchCnt = evaluator->cnt;
}

int MTD_f::mtdf(int firstGuess)
{
	/* MTD(f)窗口上下限 */
	int windowTop = WIN;
	int windowDown = -WIN;
	/* 空窗探测评估值，调整MTD(f)窗口的依据 */
	int g = firstGuess;
	/* alphaBeta算法的窗口上限 */
	int beta;
	/* 不断执行空窗探测直到窗口闭合，有hash置换表不怕重复搜索 */
	while(windowDown < windowTop)
	{
		/* 刚调整过窗口下限，更需要通过向上偏移的空窗探测调整窗口上限 */
		if (g == windowDown)
		{
			/* 在[g, g+1]区间上进行空窗探测 */
			/* alpha = g */
			beta = g + 1;
		}
		/* 刚调整过窗口上限，更需要通过向下偏移的空窗探测调整窗口下限 */
		else
		{
			/* 在[g-1, g]区间上进行空窗探测 */
			/* alpha = g - 1 */
			beta = g;
		}
		/* 利用Fail-Soft AlphaBeta算法做空窗探测 */
		/* alpha = beta - 1 */
		g = TTFAlphaBeta(beta - 1, beta, 0);
		/* 根据新评估值做窗口调整 */
		/* 新评估值小于空窗口，可以认为猜高了，实际评估值应该更低 */
		if (g < beta)
		{
			/* 根据新评估值下调MTD(f)窗口上限 */
			windowTop = g;
		}
		/* 新评估值不小于空窗口，可以认为猜低了，实际评估值应该更高 */
		else
		{
			/* 根据新评估值上调MTD(f)窗口下限 */
			windowDown = g;
		}
	}
	return g;
}

int MTD_f::TTFAlphaBeta(int alpha, int beta, unsigned char depth)
{
	/* 暂存评分 */
	int score;
	/* 尝试查置换表 */
	if (depth && (score = checkTT(alpha, beta, tryDepth)) != HASH_MISS)
	{
		return score;
	}
	/* 到达搜索树的叶结点 */
	if (depth == tryDepth)
	{
		score = evaluator->evaluate(currentState);
		updateTT(HASH_EXACT, score, tryDepth);
		return score;
	}
	/* 游戏结束 */
	if (currentState.isDone())
	{
		score = WIN + (tryDepth - depth)*100;
		updateTT(HASH_EXACT, -score, tryDepth);
		/* 结束离根状态结点越近获得越大的评分加成 */
		return -score;
	}
	/* new一个生成器，同时也是当前状态结点的走法容器 */
	SerialGenerator *gen = new SerialGenerator();
	/* 生成走法 */
	gen->generate(currentState, stateRecord);
	/* 废物利用，用MTD(f)中闲置的alpha来暂存最大走法评分 */
	alpha = -0x7fffffff;
	/* 所有走法分支逐个展开搜索 */
	for (auto &it : gen->moveList)
	{
		/* 按走法生成新局面 */
		currentState.move(it);
		/* 递归搜索，搜索目标和搜索结果符号翻转 */
		score = -TTFAlphaBeta(-beta, -alpha, depth + 1);
		/* 还原局面，给下一个走法分支的搜索做准备 */
		currentState.undo(it);
		/* 评分大于等于窗口上限，beta剪枝 */
		if (score < WIN && score >= beta)
		{
			/* 当前层为搜索树顶层 */
			if (depth == 0)
			{
				bestMove = it;
			}
			updateTT(HASH_BETA, beta, tryDepth);
			delete gen;
			return beta;
		}
		/* 更新最大评分 */
		if (score > alpha)
		{
			/* 当前层为搜索树顶层 */
			if (depth == 0)
			{
				bestMove = it;
			}
			alpha = score;
			/* 最大评分还可能更新，暂不跳出循环 */
		}
	}
	/* 空窗探测的alphaBeta算法中没到叶结点也没被beta剪枝的通通都是alpha状态 */
	updateTT(HASH_ALPHA, alpha, tryDepth);
	/* 释放走法生成器，即释放走法容器 */
	delete gen;
	return alpha;
}

int MTD_f::checkTT(int alpha, int beta, unsigned char maxDepth)
{
	if (TT.count(currentState))
	{
		TTValue &val = TT[currentState];
		/* 只提供特定深度以内的局面相关信息 */
		/* 更深的深度下遇到相同的局面应该有新的处理方式 */
		if (maxDepth <= val.maxDepth)
		{
			/* 上次遇到这种局面给出了确切评分值 */
			if (val.type == HASH_EXACT)
			{
				return val.score;
			}
			/* 上次遇到这种局面做了alpha剪枝 */
			if (val.type == HASH_ALPHA && val.score <= alpha)
			{
				return alpha;
			}
			/* 上次遇到这种局面做了beta剪枝 */
			if (val.type == HASH_BETA && val.score >= beta)
			{
				return beta;
			}
		}
	}
	return HASH_MISS;
}

void MTD_f::updateTT(std::bitset<2> type, int score, unsigned char maxDepth)
{
	/* 只有置换表未命中或者depth > val.depth，才可能调用该方法 */
	TTValue &val = TT[currentState];
	val.type = type;
	val.score = score;
	val.maxDepth = maxDepth;
	val.bestMove = bestMove;
}
