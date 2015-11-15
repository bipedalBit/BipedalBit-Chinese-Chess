#ifndef _MTD_F_H_
#define _MTD_F_H_

#include "State.h"
#include "Move.h"
#include "SearchCore.h"
#include <unordered_map>
#include <bitset>

/* hash表初始大小 */
#define HASH_SIZE 1<<20
/* hash表中结点状态信息类型 */
/* 评分恰好落在空窗中 */
#define HASH_EXACT 0
/* 评分落在空窗以下 */
#define HASH_ALPHA 1
/* 评分落在空窗以上 */
#define HASH_BETA 2
/* hash置换表未命中标识 */
#define HASH_MISS -0x7fffffff
/* hash置换表的value结构:142bit = 18Byte - 2bit */
struct TTValue
{
	/* 搜索信息类型：2bit，hash表使用的空间，能省则省 */
	std::bitset<2> type;
	/* 当前搜索的最大深度：8bit，这个用bitset的话所有现行数据结构要大改 */
	unsigned char maxDepth;
	/* 局面评分：32bit */
	int score;
	/* 当前局面下的最佳走法：8*8bit = 64bit */
	Move bestMove;
};

/* unordered_map要求提供的自定义哈希函数 */
struct KeyHash
{
	std::size_t operator()(const State &k) const;
};

class MTD_f : public SearchCore
{
public:
	MTD_f(unsigned char _maxDepth);
	/* 使用MTD(f)算法搜索一个最优走法 */
	void search(State &currentState, std::vector<State> &stateRecord);
private:
	/* 当前尝试迭代搜索的最大深度 */
	unsigned char tryDepth;
	/* unordered_map实现的置换表 */
	std::unordered_map<State, TTValue, KeyHash> TT;
	/* MTD(f)算法 */
	int mtdf(int firstGuess);
	/* 带置换表的fail-soft alpha-beta算法 */
	int TTFAlphaBeta(int alpha, int beta, unsigned char depth);
	/* 检查置换表，反馈恰当的历史评分或未命中标识 */
	int checkTT(int alpha, int beta, unsigned char maxDepth);
	/* 按特定策略记录或替换历史评分 */
	void updateTT(std::bitset<2> type, int score, unsigned char maxDepth);
};

#endif
