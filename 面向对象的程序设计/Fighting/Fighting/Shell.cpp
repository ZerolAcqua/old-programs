#include "pch.h"
#include "Shell.h"


CImage Shell::s_img;


Shell::Shell()
{
	m_inbox = true;
}

bool Shell::SSisOverlap(Shell& sh)
{
	if (IsOverLap(sh.GetPos()))
	{
		return true;
	}
	else return false;
}

void Shell::Show(CDC* pDC)
{
	UpdatePos();
	s_img.Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
}

void Shell::UpdatePos()
{
	DWORD tm = GetTickCount();
	int nDis = (tm - m_time) * m_nSpeed / 1000;
	switch (m_nDir)
	{
	case DIR_UP:
		m_rct.MoveToY(m_iniRct.top - nDis);
		break;
	case DIR_DOWN:
		m_rct.MoveToY(m_iniRct.top + nDis);
		break;
	case DIR_LEFT:
		m_rct.MoveToX(m_iniRct.left - nDis);
		break;
	case DIR_RIGHT:
		m_rct.MoveToX(m_iniRct.left + nDis);
		break;
	default:
		break;
	}
	if (m_rct.top<0 || m_rct.bottom>Y_MAX
		|| m_rct.left<0 || m_rct.right>X_MAX)
		m_inbox = false;
}

void Shell::Create(CPoint pos, int nDir, int nSpeed)
{
	m_nDir = nDir;
	m_nSpeed = nSpeed;
	int nShellSize = 4;
	m_rct = CRect(pos.x - nShellSize, pos.y - nShellSize, pos.x + nShellSize, pos.y + nShellSize);
	m_iniRct = m_rct;
	m_time = GetTickCount();
}

