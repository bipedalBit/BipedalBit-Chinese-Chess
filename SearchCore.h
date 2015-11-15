#ifndef _SEARCH_CORE_H_
#define _SEARCH_CORE_H_

#include "State.h"
#include "Move.h"
#include "Evaluator.h"
#include <ctime>
#include <vector>

/* 搜索核心，用来找出当前局面下最好的走法 */
/* 实际使用搜索类的基类，提供search()方法的实现接口 */
class SearchCore
{
public:
	/* 搜索时间，以秒为单位 */
	double searchTime;
	/* 搜索局面结点数 */
	unsigned searchCnt;
	/* 初始化搜索核心类 */
	SearchCore()
	{
		/* 为evaluator指针构造一个Evaluator类对象 */
		evaluator = new Evaluator();
	}
	/* 析构搜索核心对象 */
	~SearchCore()
	{
		/* 释放Evaluator对象 */
		delete evaluator;
	}
	/* 开始搜索，寻找一个最佳走法 */
	virtual void search(State &currentState, std::vector<State> &stateRecord) = 0;
	/* 取得最佳走法 */
	Move getBestMove() const
	{
		return bestMove;
	}
protected:
	/* 存储当前局面 */
	State currentState;
	/* 局面记录指针 */
	std::vector<State> stateRecord;
	/* 估值器指针 */
	/* 估值器要在构造搜索核心时动态构造，无法使用引用类型，只好使用指针 */
	Evaluator *evaluator;
	/* 当前搜索深度 */
	unsigned char currentDepth;
	/* 最大搜索深度 */
	unsigned char maxDepth;
	/* 最佳走法 */
	Move bestMove;
};

#endif
