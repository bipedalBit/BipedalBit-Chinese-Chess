#ifndef _MOVE_GENERATOR_H_
#define _MOVE_GENERATOR_H_

#include "Move.h"
#include "State.h"
#include <vector>

/* 走法生成器，博弈树上每个状态展开之前都需要用该方法来生成并存储可能的分支 */
/* 实际使用走法生成类的基类，提供generate()方法的实现接口 */
class MoveGenerator
{
public:
	/* 走法容器 */
	std::vector<Move> moveList;
	/* 预置容器默认大小，避免浪费太多内存 */
	MoveGenerator()
	{
		/* 将 5 */
		/* 士 4*2 = 8 */
		/* 相 4*2 = 8 */
		/* 马 8*2 = 16 */
		/* 車 (9 + 10 - 1)*2 = 36 */
		/* 炮 (9 + 10 - 1)*2 = 36 */
		/* 兵 3*5 = 15 */
		/* 合计124种走法 */
		/* 预分配124个Move的内存可以保证不必再动态开辟内存 */
		/* 实际上，不可能每个棋子都是完全自由的，这里根据经验预留80个走法的空间 */
		try
		{
			moveList.reserve(80);
		}
		catch(std::bad_alloc)
		{
			moveList.reserve(40);
		}
	}
	/* 释放走法容器占用的内存 */
	~MoveGenerator()
	{
		std::vector<Move> v;
		moveList.swap(v);
	}
	/* 根据特定的局面状态和当前行动权归属生成所有可能的走法并填充走法容器 */
	virtual void generate(State &state, std::vector<State> stateRecord) = 0;
};

#endif
