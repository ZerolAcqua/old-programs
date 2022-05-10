#pragma once
#define CORRECT 0


//单位格子的大小
//const CSize unitSize(50, 50);
/*这一块地方很有问题，
不知道为什么mytank接收不到unitsize
可能是因为构造的时候，unitSize还未初始化*/

//方向对应的数字
const int DIR_UP = 0;
const int DIR_DOWN = 1;
const int DIR_LEFT = 2;
const int DIR_RIGHT = 3;
//单元格子的大小
const int size = 50;
const int Tank_size = 45;
//范围
const int X_MAX = 600;
const int Y_MAX = 500;
//+20是为了防止出界判断的延迟出现图片残留
//具体解决还可以尝试加快判断速度
const CRect Range(CPoint(0, 0), CPoint(X_MAX + 200, Y_MAX + 40));

class Object
{
public:
	virtual void Show(CDC* pDC) {};
	void Move(int Dir);

	void SetPos(CRect rct) { m_rct = rct; m_rct.NormalizeRect(); };
	CRect GetPos() { return m_rct; };
	void setSpeed(int n) { this->m_nSpeed = n;}

	bool m_movable = true;	
	bool IsOverLap(CRect& rct);//重叠、碰撞测试基础
	bool IsOut();

	CRect m_rct;

protected:
	int m_nDir;//0-3:up\down\left\right
	int m_nSpeed;	
};



