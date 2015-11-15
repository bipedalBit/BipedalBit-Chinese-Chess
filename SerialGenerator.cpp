#include "SerialGenerator.h"

class State;
class Move;
class Position;

void SerialGenerator::generate(State &s, std::vector<State> record)
{
	unsigned char x, y, x2, y2;
	bool RTurn = s.RTurn;
	/* 红子走法 */
	if (RTurn)
	{
		/* 红帅走法 */
		if (s.isAlive(R_KING))
		{
			x = s.x(R_KING);
			y = s.y(R_KING);
			/* 尝试吃对脸将 */
			if (x == s.x(B_KING))
			{
				x2 = s.x(B_KING);
				y2 = s.y(B_KING);
				bool meet = true;
				int distance = y - y2;
				for(int i = 1 ; i < distance ; ++i)
				{
					if (s.relExist(B_KING, 0, i))
					{
						meet = false;
						break;
					}
				}
				if (meet)
				{
					moveList.push_back(Move(R_KING, B_KING, Position(x, y), Position(x2, y2)));
				}
			}
			/* 向左一步 */
			if (x > 3 && !s.relRedExist(R_KING, -1, 0))
			{
				moveList.push_back(Move(R_KING, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
			}
			/* 向右一步 */
			if (x < 5 && !s.relRedExist(R_KING, 1, 0))
			{
				moveList.push_back(Move(R_KING, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
			}
			/* 向上一步 */
			if (y > 7 && !s.relRedExist(R_KING, 0, -1))
			{
				moveList.push_back(Move(R_KING, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
			}
			/* 向下一步 */
			if (x < 9 && !s.relRedExist(R_KING, 0, 1))
			{
				moveList.push_back(Move(R_KING, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
			}
		}
		/* 红士走法 */
		for(int i = R_MANDARIN ; i < R_MANDARIN + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 3 && y > 7 && !s.relRedExist(i, -1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y - 1), Position(x, y), Position(x - 1, y - 1)));
				}
				/* 右上一步 */
				if (x < 5 && y > 7 && !s.relRedExist(i, 1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y - 1), Position(x, y), Position(x + 1, y - 1)));
				}
				/* 左下一步 */
				if (x > 3 && y < 9 && !s.relRedExist(i, -1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y + 1), Position(x, y), Position(x - 1, y + 1)));
				}
				/* 右下一步 */
				if (x < 5 && y < 9 && !s.relRedExist(i, 1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y + 1), Position(x, y), Position(x + 1, y + 1)));
				}
			}
		}
		/* 红相走法 */
		for(int i = R_ELEPHANT ; i < R_ELEPHANT + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 1 && y > 6 && !s.relRedExist(i, -2, -2) && !s.relExist(i, -1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y - 2), Position(x, y), Position(x - 2, y - 2)));
				}
				/* 右上一步 */
				if (x < 7 && y > 6 && !s.relRedExist(i, 2, -2) && !s.relExist(i, 1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y - 2), Position(x, y), Position(x + 2, y - 2)));
				}
				/* 左下一步 */
				if (x > 1 && y < 8 && !s.relRedExist(i, -2, 2) && !s.relExist(i, -1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y + 2), Position(x, y), Position(x - 2, y + 2)));
				}
				/* 右下一步 */
				if (x < 7 && y < 8 && !s.relRedExist(i, 2, 2) && !s.relExist(i, 1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y + 2), Position(x, y), Position(x + 2, y + 2)));
				}
			}
		}
		/* 红马走法 */
		for(int i = R_KNIGHT ; i < R_KNIGHT + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 1 && y > 0 && !s.relRedExist(i, -2, -1) && !s.relExist(i, -1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y - 1), Position(x, y), Position(x - 2, y - 1)));
				}
				/* 左下一步 */
				if (x > 1 && y < 9 && !s.relRedExist(i, -2, 1) && !s.relExist(i, -1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y + 1), Position(x, y), Position(x - 2, y + 1)));
				}
				/* 上左一步 */
				if (x > 0 && y > 1 && !s.relRedExist(i, -1, -2) && !s.relExist(i, 0, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y - 2), Position(x, y), Position(x - 1, y - 2)));
				}
				/* 上右一步 */
				if (x < 8 && y > 1 && !s.relRedExist(i, 1, -2) && !s.relExist(i, 0, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y - 2), Position(x, y), Position(x + 1, y - 2)));
				}
				/* 右上一步 */
				if (x < 7 && y > 0 && !s.relRedExist(i, 2, -1) && !s.relExist(i, 1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y - 1), Position(x, y), Position(x + 2, y - 1)));
				}
				/* 右下一步 */
				if (x < 7 && y < 9 && !s.relRedExist(i, 2, 1) && !s.relExist(i, 1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y + 1), Position(x, y), Position(x + 2, y + 1)));
				}
				/* 下左一步 */
				if (x > 0 && y < 8 && !s.relRedExist(i, -1, 2) && !s.relExist(i, 0, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y + 2), Position(x, y), Position(x - 1, y + 2)));
				}
				/* 下右一步 */
				if (x < 8 && y < 8 && !s.relRedExist(i, 1, 2) && !s.relExist(i, 0, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y + 2), Position(x, y), Position(x + 1, y + 2)));
				}
			}
		}
		/* 红車走法 */
		for(int i = R_ROOK ; i < R_ROOK + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 向左 */
				for(int j = 1 ; x - j >= 0 ; ++j)
				{
					if (s.relExist(i, -j, 0))
					{
						if (s.relBlackExist(i, -j, 0))
						{
							moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
				}
				/* 向右 */
				for(int j = 1 ; x + j < 9 ; ++j)
				{
					if (s.relExist(i, j, 0))
					{
						if (s.relBlackExist(i, j, 0))
						{
							moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
				}
				/* 向上 */
				for(int j = 1 ; y - j >= 0 ; ++j)
				{
					if (s.relExist(i, 0, -j))
					{
						if (s.relBlackExist(i, 0, -j))
						{
							moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
				}
				/* 向下 */
				for(int j = 1 ; y + j < 10 ; ++j)
				{
					if (s.relExist(i, 0, j))
					{
						if (s.relBlackExist(i, 0, j))
						{
							moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
				}
			}
		}
		/* 红炮走法 */
		for(int i = R_CANNON ; i < R_CANNON + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				bool over;
				/* 向左 */
				over = false;
				for(int j = 1 ; x - j >= 0 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, -j, 0))
					{
						if (over)
						{
							if (s.relBlackExist(i, -j, 0))
							{
								moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
					}
				}
				/* 向右 */
				over = false;
				for(int j = 1 ; x + j < 9 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, j, 0))
					{
						if (over)
						{
							if (s.relBlackExist(i, j, 0))
							{
								moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
					}
				}
				/* 向上 */
				over = false;
				for(int j = 1 ; y - j >= 0 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, 0, -j))
					{
						if (over)
						{
							if (s.relBlackExist(i, 0, -j))
							{
								moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
					}
				}
				/* 向下 */
				over = false;
				for(int j = 1 ; y + j < 10 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, 0, j))
					{
						if (over)
						{
							if (s.relBlackExist(i, 0, j))
							{
								moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
					}
				}
			}
		}
		/* 红兵走法 */
		for(int i = R_PAWN ; i < R_PAWN + 5 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				if (s.y(i) < 5)
				{
					/* 向左一步 */
					if (x > 0 && !s.relRedExist(i, -1, 0))
					{
						moveList.push_back(Move(i, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
					}
					/* 向右一步 */
					if (x < 8 && !s.relRedExist(i, 1, 0))
					{
						moveList.push_back(Move(i, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
					}
				}
				/* 向上一步 */
				if (y > 0 && !s.relRedExist(i, 0, -1))
				{
					moveList.push_back(Move(i, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
				}
			}
		}
	}
	/* 黑子走法 */
	else
	{
		/* 黑将走法 */
		if (s.isAlive(B_KING))
		{
			x = s.x(B_KING);
			y = s.y(B_KING);
			/* 尝试吃对脸帅 */
			if (x == s.x(R_KING))
			{
				x2 = s.x(R_KING);
				y2 = s.y(R_KING);
				bool meet = true;
				int distance = y2 - y;
				for(int i = 1 ; i < distance ; ++i)
				{
					if (s.relExist(B_KING, 0, -i))
					{
						meet = false;
						break;
					}
				}
				if (meet)
				{
					moveList.push_back(Move(B_KING, R_KING, Position(x, y), Position(x2, y2)));
				}
			}
			/* 向左一步 */
			if (x > 3 && !s.relBlackExist(B_KING, -1, 0))
			{
				moveList.push_back(Move(B_KING, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
			}
			/* 向右一步 */
			if (x < 5 && !s.relBlackExist(B_KING, 1, 0))
			{
				moveList.push_back(Move(B_KING, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
			}
			/* 向上一步 */
			if (y > 0 && !s.relBlackExist(B_KING, 0, -1))
			{
				moveList.push_back(Move(B_KING, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
			}
			/* 向下一步 */
			if (x < 2 && !s.relBlackExist(B_KING, 0, 1))
			{
				moveList.push_back(Move(B_KING, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
			}
		}
		/* 黑仕走法 */
		for(int i = B_MANDARIN ; i < B_MANDARIN + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 3 && y > 0 && !s.relBlackExist(i, -1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y - 1), Position(x, y), Position(x - 1, y - 1)));
				}
				/* 右上一步 */
				if (x < 5 && y > 0 && !s.relBlackExist(i, 1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y - 1), Position(x, y), Position(x + 1, y - 1)));
				}
				/* 左下一步 */
				if (x > 3 && y < 2 && !s.relBlackExist(i, -1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y + 1), Position(x, y), Position(x - 1, y + 1)));
				}
				/* 右下一步 */
				if (x < 5 && y < 2 && !s.relBlackExist(i, 1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y + 1), Position(x, y), Position(x + 1, y + 1)));
				}
			}
		}
		/* 黑象走法 */
		for(int i = B_ELEPHANT ; i < B_ELEPHANT + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 1 && y > 1 && !s.relBlackExist(i, -2, -2) && !s.relExist(i, -1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y - 2), Position(x, y), Position(x - 2, y - 2)));
				}
				/* 右上一步 */
				if (x < 7 && y > 1 && !s.relBlackExist(i, 2, -2) && !s.relExist(i, 1, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y - 2), Position(x, y), Position(x + 2, y - 2)));
				}
				/* 左下一步 */
				if (x > 1 && y < 3 && !s.relBlackExist(i, -2, 2) && !s.relExist(i, -1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y + 2), Position(x, y), Position(x - 2, y + 2)));
				}
				/* 右下一步 */
				if (x < 7 && y < 3 && !s.relBlackExist(i, 2, 2) && !s.relExist(i, 1, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y + 2), Position(x, y), Position(x + 2, y + 2)));
				}
			}
		}
		/* 黑马走法 */
		for(int i = B_KNIGHT ; i < B_KNIGHT + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 左上一步 */
				if (x > 1 && y > 0 && !s.relBlackExist(i, -2, -1) && !s.relExist(i, -1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y - 1), Position(x, y), Position(x - 2, y - 1)));
				}
				/* 左下一步 */
				if (x > 1 && y < 9 && !s.relBlackExist(i, -2, 1) && !s.relExist(i, -1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x - 2, y + 1), Position(x, y), Position(x - 2, y + 1)));
				}
				/* 上左一步 */
				if (x > 0 && y > 1 && !s.relBlackExist(i, -1, -2) && !s.relExist(i, 0, -1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y - 2), Position(x, y), Position(x - 1, y - 2)));
				}
				/* 上右一步 */
				if (x < 8 && y > 1 && !s.relBlackExist(i, 1, -2) && !s.relExist(i, 0, -1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y - 2), Position(x, y), Position(x + 1, y - 2)));
				}
				/* 右上一步 */
				if (x < 7 && y > 0 && !s.relBlackExist(i, 2, -1) && !s.relExist(i, 1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y - 1), Position(x, y), Position(x + 2, y - 1)));
				}
				/* 右下一步 */
				if (x < 7 && y < 9 && !s.relBlackExist(i, 2, 1) && !s.relExist(i, 1, 0))
				{
					moveList.push_back(Move(i, s.getNo(x + 2, y + 1), Position(x, y), Position(x + 2, y + 1)));
				}
				/* 下左一步 */
				if (x > 0 && y < 8 && !s.relBlackExist(i, -1, 2) && !s.relExist(i, 0, 1))
				{
					moveList.push_back(Move(i, s.getNo(x - 1, y + 2), Position(x, y), Position(x - 1, y + 2)));
				}
				/* 下右一步 */
				if (x < 8 && y < 8 && !s.relBlackExist(i, 1, 2) && !s.relExist(i, 0, 1))
				{
					moveList.push_back(Move(i, s.getNo(x + 1, y + 2), Position(x, y), Position(x + 1, y + 2)));
				}
			}
		}
		/* 黑車走法 */
		for(int i = B_ROOK ; i < B_ROOK + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				/* 向左 */
				for(int j = 1 ; x - j >= 0 ; ++j)
				{
					if (s.relExist(i, -j, 0))
					{
						if (s.relRedExist(i, -j, 0))
						{
							moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
				}
				/* 向右 */
				for(int j = 1 ; x + j < 9 ; ++j)
				{
					if (s.relExist(i, j, 0))
					{
						if (s.relRedExist(i, j, 0))
						{
							moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
				}
				/* 向上 */
				for(int j = 1 ; y - j >= 0 ; ++j)
				{
					if (s.relExist(i, 0, -j))
					{
						if (s.relRedExist(i, 0, -j))
						{
							moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
				}
				/* 向下 */
				for(int j = 1 ; y + j < 10 ; ++j)
				{
					if (s.relExist(i, 0, j))
					{
						if (s.relRedExist(i, 0, j))
						{
							moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
						}
						break;
					}
					moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
				}
			}
		}
		/* 黑炮走法 */
		for(int i = B_CANNON ; i < B_CANNON + 2 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				bool over;
				/* 向左 */
				over = false;
				for(int j = 1 ; x - j >= 0 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, -j, 0))
					{
						if (over)
						{
							if (s.relRedExist(i, -j, 0))
							{
								moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
					}
				}
				/* 向右 */
				over = false;
				for(int j = 1 ; x + j < 9 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, j, 0))
					{
						if (over)
						{
							if (s.relRedExist(i, j, 0))
							{
								moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
					}
				}
				/* 向上 */
				over = false;
				for(int j = 1 ; y - j >= 0 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, 0, -j))
					{
						if (over)
						{
							if (s.relRedExist(i, 0, -j))
							{
								moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
					}
				}
				/* 向下 */
				over = false;
				for(int j = 1 ; y + j < 10 ; ++j)
				{
					/* 尝试吃子 */
					if (s.relExist(i, 0, j))
					{
						if (over)
						{
							if (s.relRedExist(i, 0, j))
							{
								moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
							}
							break;
						}
						else
						{
							over = true;
						}
					}
					/* 不吃子 */
					if (!over)
					{
						moveList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
					}
				}
			}
		}
		/* 黑卒走法 */
		for(int i = B_PAWN ; i < B_PAWN + 5 ; ++i)
		{
			if (s.isAlive(i))
			{
				x = s.x(i);
				y = s.y(i);
				if (s.y(i) > 4)
				{
					/* 向左一步 */
					if (x > 0 && !s.relBlackExist(i, -1, 0))
					{
						moveList.push_back(Move(i, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
					}
					/* 向右一步 */
					if (x < 8 && !s.relBlackExist(i, 1, 0))
					{
						moveList.push_back(Move(i, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
					}
				}
				/* 向下一步 */
				if (y < 9 && !s.relBlackExist(i, 0, 1))
				{
					moveList.push_back(Move(i, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
				}
			}
		}
	}
	/* 循环走法不合法 */
	std::vector<Move>::iterator begin = moveList.begin();
	int i;
	for(auto r : record)
	{
		i = 0;
		for(auto it : moveList)
		{
			s.move(it);
			if (s == r)
			{
				s.undo(it);
				moveList.erase(begin + i);
				break;
			}
			s.undo(it);
			++i;
		}
	}
}
