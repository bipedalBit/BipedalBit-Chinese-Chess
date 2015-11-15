#include "AlphaBeta.h"
#include "SerialGenerator.h"
#include <ctime>

class State;

AlphaBeta::AlphaBeta(unsigned char _maxDepth)
{
	maxDepth = _maxDepth;
}

void AlphaBeta::search(State &s, std::vector<State> &record)
{
	/* 每次搜索前评估结点数清零 */
	evaluator->cnt = 0;
	/* 复制局面状态 */
	currentState = s;
	/* 复制局面记录指针 */
	stateRecord = record;
	/* 开始计时 */
	clock_t t0 = clock();
	/* 执行alphaBeta算法 */
	alphaBeta(-0x7fffffff, 0x7fffffff, 0);
	/* 停止计时 */
	clock_t t1 = clock() - t0;
	/* 记录评估结点数 */
	searchCnt = evaluator->cnt;
	/* 记录搜索用时 */
	searchTime = (double)t1/CLOCKS_PER_SEC;
}

int AlphaBeta::alphaBeta(int alpha, int beta, unsigned char depth)
{
	/* 暂存评分 */
	int score;
	/* 到达搜索树的叶结点 */
	if (depth == maxDepth)
	{
		score = evaluator->evaluate(currentState);
		return score;
	}
	/* 游戏结束 */
	if (currentState.isDone())
	{
		score = WIN + (maxDepth - depth)*100;
		/* 结束离根状态结点越近获得越大的评分加成 */
		return -score;
	}
	/* new一个生成器，同时也是当前状态结点的走法容器 */
	SerialGenerator *gen = new SerialGenerator();
	/* 生成走法 */
	gen->generate(currentState, stateRecord);
	/* 所有走法分支逐个展开搜索 */
	for (auto &it : gen->moveList)
	{
		/* 按走法生成新局面 */
		currentState.move(it);
		/* 递归搜索，搜索目标和搜索结果符号翻转 */
		score = -alphaBeta(-beta, -alpha, depth + 1);
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
			delete gen;
			return beta;
		}
		/* 评分大于窗口下限，更新窗口下限 */
		/* alpha是变量，放着也是浪费，就拿来存最大评分 */
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
	/* 释放走法生成器，即释放走法容器 */
	delete gen;
	return alpha;
}
