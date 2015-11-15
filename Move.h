#ifndef _MOVE_H_
#define _MOVE_H_

#include "Position.h"

/* 胜利评分 */
#define WIN 0x100000

/* 存储走法的数据结构：8字节 */
class Move
{
public:
	/* 棋子序号 */
	unsigned char chessNo;
	/* 被吃掉棋子序号或空序号 */
	unsigned char eatenNo;
	/* 原位置 */
	Position from;
	/* 目的位置 */
	Position to;
	/* 空走法构造方法 */
	/* 这样构造的空走法将不会从走法生成器中产生 */
	Move()
	{
		/* 就不为了一个NONE特意包含State.h了 */
		chessNo = 0xff;
	}
	/* 走法构造方法 */
	Move(unsigned char _chessNo, unsigned char _eatenNo, Position _from, Position _to)
	{
		chessNo = _chessNo;
		eatenNo = _eatenNo;
		from = _from;
		to = _to;
	}
	bool operator == (const Move &move) const
	{
		if (chessNo == move.chessNo && eatenNo == move.eatenNo && from == move.from && to == move.to)
		{
			return true;
		}
		return false;
	}
};

#endif
