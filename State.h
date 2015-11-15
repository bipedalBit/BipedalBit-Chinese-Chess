#ifndef _STATE_H_
#define _STATE_H_

#include "Position.h"
#include "Move.h"

/* 棋子序号 */
/* 红帅 */
#define R_KING 0
/* 红仕序号起点（序号个数2） */
#define R_MANDARIN 1
/* 红相序号起点（序号个数2） */
#define R_ELEPHANT 3
/* 红马序号起点（序号个数2） */
#define R_KNIGHT 5
/* 红車序号起点（序号个数2） */
#define R_ROOK 7
/* 红炮序号起点（序号个数2） */
#define R_CANNON 9
/* 红兵序号起点（序号个数5） */
#define R_PAWN 11
/* 红子序号起点 */
#define R_BEGIN R_KING
/* 红子序号终点 */
#define R_END R_PAWN+4
/* 黑将 */
#define B_KING 16
/* 黑士序号起点（序号个数2） */
#define B_MANDARIN 17
/* 黑象序号起点（序号个数2） */
#define B_ELEPHANT 19
/* 黑马序号起点（序号个数2） */
#define B_KNIGHT 21
/* 黑車序号起点（序号个数2） */
#define B_ROOK 23
/* 黑炮序号起点（序号个数2） */
#define B_CANNON 25
/* 黑卒序号起点（序号个数5） */
#define B_PAWN 27
/* 黑子序号起点 */
#define B_BEGIN B_KING
/* 黑子序号终点 */
#define B_END B_PAWN+4
/* 坐标数组中表示棋子已被吃 */
#define DEAD 0xff
/* 棋子数组中表示坐标无棋子占用 */
#define NONE DEAD

/* 局面状态类：32+90=122字节 */
/* 这个类负责记录局面状态，包括局面的棋子索引的状态和坐标索引的状态。 */
/* 坐标索引的状态提供坐标优先的快速状态查询； */
/* 棋子索引的状态提供棋子优先的快速状态查询。 */
class State
{
public:
	/* 当前局面下红方是否持有行动权 */
	bool RTurn;
	/* 根据中国象棋规则初始化局面状态 */
	State();
	/* 根据走法更新棋盘状态 */
	void move(const Move &m);
	/* 还原按走法还原更新前的棋盘状态 */
	void undo(const Move &m);
	/* 获取特定棋子行号，即棋子y坐标 */
	unsigned char y(unsigned char chessNo) const;
	/* 获取特定棋子列号，即棋子x坐标 */
	unsigned char x(unsigned char chessNo) const;
	/* 获取特定坐标上的棋子序号或空序号 */
	unsigned char getNo(unsigned char x, unsigned char y) const;
	/* 存活判定 */
	bool isAlive(unsigned char chessNo) const;
	/* 红子判定 */
	bool isRed(unsigned char chessNo) const;
	/* 黑子判定 */
	bool isBlack(unsigned char chessNo) const;
	/* 友方判定 */
	bool isFriend(unsigned char chessNo1, unsigned char chessNo2) const;
	/* 特定棋子相对位置处是否有子 */
	bool relExist(unsigned char chessNo, char x, char y) const;
	/* 特定棋子相对位置处是否有红子 */
	bool relRedExist(unsigned char chessNo, char x, char y) const;
	/* 特定棋子相对位置处是否有黑子 */
	bool relBlackExist(unsigned char chessNo, char x, char y) const;
	/* 判断当前棋局是否已经结束，即一方的将/帅已经被吃 */
	bool isDone() const;
	/* 定义一个相等运算符，unordered_map要用 */
	bool operator == (const State state) const;
	/* 允许KeyHash类访问私有成员 */
	friend class KeyHash;
private:
	/* 使用双向索引，加快两个方向的查询速度 */
	/* 棋子序号索引的坐标数组：32字节 */
	Position posList[32];
	/* 棋盘坐标索引的棋子数组：90字节 */
	/* 二维数组的每个单元为一个8位整型数，用来表示坐标占用情况 */
	/* 即与坐标数组通用的棋子序号和一个额外的表示空的序号0xff */
	unsigned char board[10][9];
};

#endif
