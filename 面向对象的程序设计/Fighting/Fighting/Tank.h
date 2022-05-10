#pragma once
#include "Object.h"
#include "Shell.h"

class Tank:public Object
{
	//这里只定义了Show方法，fire方法在子类中定义
public:
	Tank();
	int m_life;
	virtual void Show(CDC* pDC);
	virtual Shell fire();
	//生命-1
	virtual void lifedecrease()
	{
		m_life--;
	}
	bool TisOverlap(CRect rct);
	bool TOisOverlap(Object& ob);
};

