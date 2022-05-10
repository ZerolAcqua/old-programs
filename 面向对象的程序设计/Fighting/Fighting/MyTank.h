#pragma once
#include "Tank.h"

class MyTank : public Tank
{
public:
	Shell Fire();
	MyTank();
	bool m_move = false;


	bool respan();
	bool gameover();
	bool show_life(CDC* pDC,CRect rct);
	void Show(CDC* pDC);
	void setDir(int i);
	void myMove();
	static CImage m_img[4];
private:
	CPoint m_respanpos;
};

