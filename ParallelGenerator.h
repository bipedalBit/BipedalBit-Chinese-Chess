#ifndef _PARALLEL_GENERATOR_H_
#define _PARALLEL_GENERATOR_H_

#include "MoveGenerator.h"

class State;

/* 并行走法生成器类，是对走法生成器基类的实现 */
class ParallelGenerator
{
public:
	/* 走法生成方法采用并行方式 */
	void generate(State &state, std::vector<State> stateRecord);
};

#endif
