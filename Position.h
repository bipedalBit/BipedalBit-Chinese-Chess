#ifndef _POSITION_H_
#define _POSITION_H_

/* 棋子位置/坐标类：1字节 */
/* 注意这个类型比指针（4字节）还小，尽量作为变量形参而非引用参数 */
class Position
{
public:
	/* 默认构造函数 */
	Position(){}
	/* 按特定坐标构造位置实例 */
	Position(unsigned char _x, unsigned char _y):pos(_y << 4 | _x){}
	/* 取横坐标/列号 */
	unsigned char x() const
	{
		return pos & 0x0f;
	}
	/* 取纵坐标/行号 */
	unsigned char y() const
	{
		return pos >> 4;
	}
	/* 获取当前棋子是否已被杀死 */
	bool dead() const
	{
		return pos == 0xff;
	}
	/* 标记当前棋子的坐标为已被杀死状态 */
	void kill()
	{
		pos = 0xff;
	}
	bool operator == (const Position p) const
	{
		return p.pos == pos;
	}
	/* 定义一个不相等运算符，State类要用 */
	bool operator != (const Position p) const
	{
		return p.pos != pos;
	}
	/* 允许KeyHash类访问私有成员 */
	friend class KeyHash;
private:
	/* 位置的存储结构 */
	/* 高4位存纵坐标/行号 */
	/* 低4位存横坐标/列号 */
	/* 0xff表示当前棋子已死 */
	unsigned char pos;
};

#endif
