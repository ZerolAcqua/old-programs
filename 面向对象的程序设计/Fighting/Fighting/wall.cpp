#include "pch.h"
#include "wall.h"


CImage wall::w_img[6];

wall::wall(CPoint pt,int mode)
{
	CRect temp(pt, CSize(size, size));
	type = mode;
	m_movable = false;
	m_rct = temp;
	switch (type)
	{
	case BRICK:
		destroyable = true;
		break;
	case GRASS:
		ShellPass = true;
		TankPass = true;
		break;
	case WATER:
		ShellPass = true;
		break;
	case WOOD:
		destroyable = true;
		m_movable = true;
		break;
	case BASE:
		destroyable = true;
		TankPass = true;
		break;
	}
};

void wall::Show(CDC* pDC)
{
	w_img[type].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
}

