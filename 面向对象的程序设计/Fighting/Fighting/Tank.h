#pragma once
#include "Object.h"
#include "Shell.h"

class Tank:public Object
{
	//����ֻ������Show������fire�����������ж���
public:
	Tank();
	int m_life;
	virtual void Show(CDC* pDC);
	virtual Shell fire();
	//����-1
	virtual void lifedecrease()
	{
		m_life--;
	}
	bool TisOverlap(CRect rct);
	bool TOisOverlap(Object& ob);
};

