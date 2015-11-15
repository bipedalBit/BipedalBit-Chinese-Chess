#include "State.h"

class Position;
class Move;

State::State()
{
	/* 初始化坐标数组 */
	posList[R_KING] = Position(4, 9);
	posList[R_MANDARIN] = Position(3, 9);
	posList[R_MANDARIN+1] = Position(5, 9);
	posList[R_ELEPHANT] = Position(2, 9);
	posList[R_ELEPHANT+1] = Position(6, 9);
	posList[R_KNIGHT] = Position(1, 9);
	posList[R_KNIGHT+1] = Position(7, 9);
	posList[R_ROOK] = Position(0, 9);
	posList[R_ROOK+1] = Position(8, 9);
	posList[R_CANNON] = Position(1, 7);
	posList[R_CANNON+1] = Position(7, 7);
	posList[R_PAWN] = Position(0, 6);
	posList[R_PAWN+1] = Position(2, 6);
	posList[R_PAWN+2] = Position(4, 6);
	posList[R_PAWN+3] = Position(6, 6);
	posList[R_PAWN+4] = Position(8, 6);
	posList[B_KING] = Position(4, 0);
	posList[B_MANDARIN] = Position(3, 0);
	posList[B_MANDARIN+1] = Position(5, 0);
	posList[B_ELEPHANT] = Position(2, 0);
	posList[B_ELEPHANT+1] = Position(6, 0);
	posList[B_KNIGHT] = Position(1, 0);
	posList[B_KNIGHT+1] = Position(7, 0);
	posList[B_ROOK] = Position(0, 0);
	posList[B_ROOK+1] = Position(8, 0);
	posList[B_CANNON] = Position(1, 2);
	posList[B_CANNON+1] = Position(7, 2);
	posList[B_PAWN] = Position(0, 3);
	posList[B_PAWN+1] = Position(2, 3);
	posList[B_PAWN+2] = Position(4, 3);
	posList[B_PAWN+3] = Position(6, 3);
	posList[B_PAWN+4] = Position(8, 3);
	/* 初始化棋子数组 */
	unsigned char _board[10][9] =
	{
		B_ROOK, B_KNIGHT, B_ELEPHANT, B_MANDARIN, B_KING, B_MANDARIN+1, B_ELEPHANT+1, B_KNIGHT+1, B_ROOK+1,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		NONE, B_CANNON, NONE, NONE, NONE, NONE, NONE, B_CANNON+1, NONE,
		B_PAWN ,NONE, B_PAWN+1, NONE, B_PAWN+2, NONE, B_PAWN+3, NONE, B_PAWN+4,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		R_PAWN ,NONE, R_PAWN+1, NONE, R_PAWN+2, NONE, R_PAWN+3, NONE, R_PAWN+4,
		NONE, R_CANNON, NONE, NONE, NONE, NONE, NONE, R_CANNON+1, NONE,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		R_ROOK, R_KNIGHT, R_ELEPHANT, R_MANDARIN, R_KING, R_MANDARIN+1, R_ELEPHANT+1, R_KNIGHT+1, R_ROOK+1,
	};
	for(int i = 0 ; i < 10 ; ++i)
	{
		for(int j = 0 ; j < 9 ; ++j)
		{
			board[i][j] = _board[i][j];
		}
	}
	/* 默认红方先行 */
	RTurn = true;
}

void State::move(const Move &m)
{
	if (m.chessNo == NONE)
	{
		return;
	}
	/* 更新坐标数组 */
	posList[m.chessNo] = m.to;
	if (m.eatenNo != NONE)
	{
		posList[m.eatenNo].kill();
	}
	/* 更新棋子数组 */
	board[m.to.y()][m.to.x()] = m.chessNo;
	board[m.from.y()][m.from.x()] = NONE;
	/* 交换行动权 */
	RTurn = !RTurn;
}

void State::undo(const Move &m)
{
	/* 还原坐标数组 */
	posList[m.chessNo] = m.from;
	if (m.eatenNo != NONE)
	{
		posList[m.eatenNo] = m.to;
	}
	/* 还原棋子数组 */
	board[m.from.y()][m.from.x()] = m.chessNo;
	board[m.to.y()][m.to.x()] = m.eatenNo;
	/* 交换行动权 */
	RTurn = !RTurn;
}

unsigned char State::x(unsigned char chessNo) const
{
	return posList[chessNo].x();
}

unsigned char State::y(unsigned char chessNo) const
{
	return posList[chessNo].y();
}

unsigned char State::getNo(unsigned char x, unsigned char y) const
{
	return board[y][x];
}

bool State::isAlive(unsigned char chessNo) const
{
	return !posList[chessNo].dead();
}

bool State::isRed(unsigned char chessNo) const
{
	return chessNo >= R_BEGIN && chessNo <= R_END;
}

bool State::isBlack(unsigned char chessNo) const
{
	return chessNo >= B_BEGIN && chessNo <= B_END;
}

bool State::isFriend(unsigned char chessNo1, unsigned char chessNo2) const
{
	return (isRed(chessNo1) && isRed(chessNo2)) || (isBlack(chessNo1) && isBlack(chessNo2));
}

bool State::relExist(unsigned char chessNo, char x, char y) const
{
	return board[posList[chessNo].y() + y][posList[chessNo].x() + x] != NONE;
}

bool State::relRedExist(unsigned char chessNo, char x, char y) const
{
	return isRed(board[posList[chessNo].y() + y][posList[chessNo].x() + x]);
}

bool State::relBlackExist(unsigned char chessNo, char x, char y) const
{
	return isBlack(board[posList[chessNo].y() + y][posList[chessNo].x() + x]);
}

bool State::isDone() const
{
	return posList[R_KING].dead() || posList[B_KING].dead();
}

bool State::operator == (const State s) const
{
	for (int i = 0 ; i < 32 ; ++i)
	{
		if (posList[i] != s.posList[i])
		{
			return false;
		}
	}
	return RTurn == s.RTurn;
}
