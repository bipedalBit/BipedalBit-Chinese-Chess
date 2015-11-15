#ifndef _SERIAL_GENERATOR_H_
#define _SERIAL_GENERATOR_H_

#include "MoveGenerator.h"

class State;

/* 串行走法生成器类，是对走法生成器基类的实现 */
class SerialGenerator: public MoveGenerator
{
public:
	/* 走法生成方法采用串行方式 */
	void generate(State &state, std::vector<State> stateRecord);
};

#endif
