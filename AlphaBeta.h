#ifndef _ALPHA_BETA_H_
#define _ALPHA_BETA_H_

#include "SearchCore.h"

class State;

class AlphaBeta : public SearchCore
{
public:
	AlphaBeta(unsigned char _maxDepth);
	/* 使用alphaBeta算法搜索一个最优走法 */
	void search(State &currentState, std::vector<State> &stateRecord);
private:
	int alphaBeta(int alpha, int beta, unsigned char depth);
};

#endif
