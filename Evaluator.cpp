#include "Evaluator.h"
#include <vector>

class State;
class Move;

Evaluator::Evaluator()
{
	cnt = 0;
	/* 将 5 */
	/* 士 4*2 = 8 */
	/* 相 4*2 = 8 */
	/* 马 8*2 = 16 */
	/* 車 (9 + 10 - 1)*2 = 36 */
	/* 炮 (9 + 10 - 1)*2 = 36 */
	/* 兵 3*5 = 15 */
	/* 合计124种走法 */
	/* 双方合计248种走法 */
	coverList.reserve(248);
}

void Evaluator::generate(const State &s)
{
	unsigned char x, y, x2, y2;
	/* 红子走法 */
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
				if (s.relExist(R_KING, 0, -i))
				{
					meet = false;
					break;
				}
			}
			if (meet)
			{
				coverList.push_back(Move(R_KING, B_KING, Position(x, y), Position(x2, y2)));
			}
		}
		/* 向左一步 */
		if (x > 3)
		{
			coverList.push_back(Move(R_KING, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
		}
		/* 向右一步 */
		if (x < 5)
		{
			coverList.push_back(Move(R_KING, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
		}
		/* 向上一步 */
		if (y > 7)
		{
			coverList.push_back(Move(R_KING, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
		}
		/* 向下一步 */
		if (y < 9)
		{
			coverList.push_back(Move(R_KING, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
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
			if (x > 3 && y > 7)
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y - 1), Position(x, y), Position(x - 1, y - 1)));
			}
			/* 右上一步 */
			if (x < 5 && y > 7)
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y - 1), Position(x, y), Position(x + 1, y - 1)));
			}
			/* 左下一步 */
			if (x > 3 && y < 9)
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y + 1), Position(x, y), Position(x - 1, y + 1)));
			}
			/* 右下一步 */
			if (x < 5 && y < 9)
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y + 1), Position(x, y), Position(x + 1, y + 1)));
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
			if (x > 1 && y > 6 && !s.relExist(i, -1, -1))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y - 2), Position(x, y), Position(x - 2, y - 2)));
			}
			/* 右上一步 */
			if (x < 7 && y > 6 && !s.relExist(i, 1, -1))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y - 2), Position(x, y), Position(x + 2, y - 2)));
			}
			/* 左下一步 */
			if (x > 1 && y < 8 && !s.relExist(i, -1, 1))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y + 2), Position(x, y), Position(x - 2, y + 2)));
			}
			/* 右下一步 */
			if (x < 7 && y < 8 && !s.relExist(i, 1, 1))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y + 2), Position(x, y), Position(x + 2, y + 2)));
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
			if (x > 1 && y > 0 && !s.relExist(i, -1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y - 1), Position(x, y), Position(x - 2, y - 1)));
			}
			/* 左下一步 */
			if (x > 1 && y < 9 && !s.relExist(i, -1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y + 1), Position(x, y), Position(x - 2, y + 1)));
			}
			/* 上左一步 */
			if (x > 0 && y > 1 && !s.relExist(i, 0, -1))
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y - 2), Position(x, y), Position(x - 1, y - 2)));
			}
			/* 上右一步 */
			if (x < 8 && y > 1 && !s.relExist(i, 0, -1))
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y - 2), Position(x, y), Position(x + 1, y - 2)));
			}
			/* 右上一步 */
			if (x < 7 && y > 0 && !s.relExist(i, 1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y - 1), Position(x, y), Position(x + 2, y - 1)));
			}
			/* 右下一步 */
			if (x < 7 && y < 9 && !s.relExist(i, 1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y + 1), Position(x, y), Position(x + 2, y + 1)));
			}
			/* 下左一步 */
			if (x > 0 && y < 8 && !s.relExist(i, 0, 1))
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y + 2), Position(x, y), Position(x - 1, y + 2)));
			}
			/* 下右一步 */
			if (x < 8 && y < 8 && !s.relExist(i, 0, 1))
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y + 2), Position(x, y), Position(x + 1, y + 2)));
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
				coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
				if (s.relExist(i, -j, 0))
				{
					break;
				}
			}
			/* 向右 */
			for(int j = 1 ; x + j < 9 ; ++j)
			{
				coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
				if (s.relExist(i, j, 0))
				{
					break;
				}
			}
			/* 向上 */
			for(int j = 1 ; y - j >= 0 ; ++j)
			{
				coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
				if (s.relExist(i, 0, -j))
				{
					break;
				}
			}
			/* 向下 */
			for(int j = 1 ; y + j < 10 ; ++j)
			{
				coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
				if (s.relExist(i, 0, j))
				{
					break;
				}
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
						coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
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
					coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
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
						coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
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
					coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
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
						coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
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
					coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
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
						coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
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
					coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
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
				if (x > 0)
				{
					coverList.push_back(Move(i, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
				}
				/* 向右一步 */
				if (x < 8)
				{
					coverList.push_back(Move(i, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
				}
			}
			/* 向上一步 */
			if (y > 0)
			{
				coverList.push_back(Move(i, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
			}
		}
	}
	/* 黑子走法 */
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
				if (s.relExist(B_KING, 0, i))
				{
					meet = false;
					break;
				}
			}
			if (meet)
			{
				coverList.push_back(Move(B_KING, R_KING, Position(x, y), Position(x2, y2)));
			}
		}
		/* 向左一步 */
		if (x > 3)
		{
			coverList.push_back(Move(B_KING, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
		}
		/* 向右一步 */
		if (x < 5)
		{
			coverList.push_back(Move(B_KING, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
		}
		/* 向上一步 */
		if (y > 0)
		{
			coverList.push_back(Move(B_KING, s.getNo(x, y - 1), Position(x, y), Position(x, y - 1)));
		}
		/* 向下一步 */
		if (y < 2)
		{
			coverList.push_back(Move(B_KING, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
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
			if (x > 3 && y > 0)
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y - 1), Position(x, y), Position(x - 1, y - 1)));
			}
			/* 右上一步 */
			if (x < 5 && y > 0)
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y - 1), Position(x, y), Position(x + 1, y - 1)));
			}
			/* 左下一步 */
			if (x > 3 && y < 2)
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y + 1), Position(x, y), Position(x - 1, y + 1)));
			}
			/* 右下一步 */
			if (x < 5 && y < 2)
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y + 1), Position(x, y), Position(x + 1, y + 1)));
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
			if (x > 1 && y > 1 && !s.relExist(i, -1, -1))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y - 2), Position(x, y), Position(x - 2, y - 2)));
			}
			/* 右上一步 */
			if (x < 7 && y > 1 && !s.relExist(i, 1, -1))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y - 2), Position(x, y), Position(x + 2, y - 2)));
			}
			/* 左下一步 */
			if (x > 1 && y < 3 && !s.relExist(i, -1, 1))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y + 2), Position(x, y), Position(x - 2, y + 2)));
			}
			/* 右下一步 */
			if (x < 7 && y < 3 && !s.relExist(i, 1, 1))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y + 2), Position(x, y), Position(x + 2, y + 2)));
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
			if (x > 1 && y > 0 && !s.relExist(i, -1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y - 1), Position(x, y), Position(x - 2, y - 1)));
			}
			/* 左下一步 */
			if (x > 1 && y < 9 && !s.relExist(i, -1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x - 2, y + 1), Position(x, y), Position(x - 2, y + 1)));
			}
			/* 上左一步 */
			if (x > 0 && y > 1 && !s.relExist(i, 0, -1))
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y - 2), Position(x, y), Position(x - 1, y - 2)));
			}
			/* 上右一步 */
			if (x < 8 && y > 1 && !s.relExist(i, 0, -1))
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y - 2), Position(x, y), Position(x + 1, y - 2)));
			}
			/* 右上一步 */
			if (x < 7 && y > 0 && !s.relExist(i, 1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y - 1), Position(x, y), Position(x + 2, y - 1)));
			}
			/* 右下一步 */
			if (x < 7 && y < 9 && !s.relExist(i, 1, 0))
			{
				coverList.push_back(Move(i, s.getNo(x + 2, y + 1), Position(x, y), Position(x + 2, y + 1)));
			}
			/* 下左一步 */
			if (x > 0 && y < 8 && !s.relExist(i, 0, 1))
			{
				coverList.push_back(Move(i, s.getNo(x - 1, y + 2), Position(x, y), Position(x - 1, y + 2)));
			}
			/* 下右一步 */
			if (x < 8 && y < 8 && !s.relExist(i, 0, 1))
			{
				coverList.push_back(Move(i, s.getNo(x + 1, y + 2), Position(x, y), Position(x + 1, y + 2)));
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
					coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
					break;
				}
				coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
			}
			/* 向右 */
			for(int j = 1 ; x + j < 9 ; ++j)
			{
				if (s.relExist(i, j, 0))
				{
					coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
					break;
				}
				coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
			}
			/* 向上 */
			for(int j = 1 ; y - j >= 0 ; ++j)
			{
				if (s.relExist(i, 0, -j))
				{
					coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
					break;
				}
				coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
			}
			/* 向下 */
			for(int j = 1 ; y + j < 10 ; ++j)
			{
				if (s.relExist(i, 0, j))
				{
					coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
					break;
				}
				coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
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
						coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
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
					coverList.push_back(Move(i, s.getNo(x - j, y), Position(x, y), Position(x - j, y)));
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
						coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
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
					coverList.push_back(Move(i, s.getNo(x + j, y), Position(x, y), Position(x + j, y)));
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
						coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
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
					coverList.push_back(Move(i, s.getNo(x, y - j), Position(x, y), Position(x, y - j)));
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
						coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
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
					coverList.push_back(Move(i, s.getNo(x, y + j), Position(x, y), Position(x, y + j)));
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
				if (x > 0)
				{
					coverList.push_back(Move(i, s.getNo(x - 1, y), Position(x, y), Position(x - 1, y)));
				}
				/* 向右一步 */
				if (x < 8)
				{
					coverList.push_back(Move(i, s.getNo(x + 1, y), Position(x, y), Position(x + 1, y)));
				}
			}
			/* 向下一步 */
			if (y < 9)
			{
				coverList.push_back(Move(i, s.getNo(x, y + 1), Position(x, y), Position(x, y + 1)));
			}
		}
	}
}

int Evaluator::evaluate(State &s)
{
	/* 初始化几个暂存评分的容器 */
	for(int i = 0 ; i < 32 ; ++i)
	{
		threatenScore[i] = protectScore[i] = false;
		chessScore[i] = s.isAlive(i) ? BASE_VAL[i] : 0;
	}
	/* 初始化棋子势力覆盖方式容器 */
	coverList.clear();
	/* 生成棋子势力覆盖方式 */
	generate(s);
	/* 遍历棋子势力覆盖范围，给棋子对局面施加的影响打分 */
	std::vector<Move>::iterator it;
	std::vector<Move>::iterator begin = coverList.begin();
	std::vector<Move>::iterator end = coverList.end();
	for (it = begin; it != end; ++it)
	{
		/* 目标位置为友方棋子 */
		if (s.isFriend(it->chessNo, it->eatenNo))
		{
			/* 目标棋子增加保护评分 */
			protectScore[it->eatenNo] = true;
		}
		else
		{
			/* 当前棋子增加自由度评分 */
			chessScore[it->chessNo] += FREE_UNIT[it->chessNo];
			/* 目标位置为敌方棋子 */
			if (it->eatenNo != NONE)
			{
				/* 对目标位置棋子施加威胁加成 */
				threatenScore[it->eatenNo] = true;
			}
		}
	}
	/* 统计棋子评分，并给走法打分 */
	int redScore = 0;
	int blackScore = 0;
	/* 威胁/保护基础分 */
	int controlUnitScore;
	for(int i = 0 ; i < 32 ; ++i)
	{
		if (s.isAlive(i))
		{
			controlUnitScore = BASE_VAL[i]/16;
			/* 特殊位置加成 */
			switch(i)
			{
				case R_KNIGHT: chessScore[i] += ADD_R_KNIGHT[s.y(i)][s.x(i)]; break;
				case B_KNIGHT: chessScore[i] += ADD_B_KNIGHT[s.y(i)][s.x(i)]; break;
				case R_CANNON: chessScore[i] += ADD_R_CANNON[s.y(i)][s.x(i)]; break;
				case B_CANNON: chessScore[i] += ADD_B_CANNON[s.y(i)][s.x(i)]; break;
				case R_PAWN: chessScore[i] += ADD_R_PAWN[s.y(i)][s.x(i)]; break;
				case B_PAWN: chessScore[i] += ADD_B_PAWN[s.y(i)][s.x(i)]; break;
			}
			/* 受到威胁 */
			if (threatenScore[i])
			{
				/* 行动权归敌方 */
				if (s.isRed(i) ^ s.RTurn)
				{
					/* 有保护时威胁影响减小 */
					if (protectScore[i])
					{
						chessScore[i] -= controlUnitScore;
					}
					/* 没保护时威胁很大 */
					else
					{
						chessScore[i] -= controlUnitScore*10;
					}
				}
				/* 行动权归己方 */
				else
				{
					/* 被将军时特判 */
					if (i == R_KING)
					{
						chessScore[i] -= 20;
					}
					else
					{
						/* 有保护时威胁影响减小 */
						if (protectScore[i])
						{
							chessScore[i] -= controlUnitScore;
						}
						/* 没保护时威胁较大 */
						else
						{
							chessScore[i] -= controlUnitScore*2;
						}
					}
				}
			}
			/* 没受威胁还受到保护 */
			else if (protectScore[i])
			{
				/* 略作加分 */
				chessScore[i] += 5;
			}
			/* 并入红/黑方总分 */
			if (s.isRed(i))
			{
				redScore += chessScore[i];
			}
			else
			{
				blackScore += chessScore[i];
			}
		}
	}
	/* 评估结点数加1 */
	++cnt;
	/* 给走法打分 */
	return s.RTurn ? (redScore - blackScore) : (blackScore - redScore);
}
