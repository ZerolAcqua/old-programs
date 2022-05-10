#pragma once
#include "Object.h"

class Shell :public Object
{
public:
	Shell();
	CImage static s_img;
	bool m_inbox;
public:
	void Show(CDC* pDC);
	void Create(CPoint pos, int nDir, int nSpeed);
	bool SSisOverlap(Shell& sh);
protected:
	void UpdatePos();
protected:
	DWORD m_time;
	CRect m_iniRct;
};



