#include "pch.h"
#include "Object.h"


void Object::Move(int Dir)
{
	//这里移动是将来的移动，能否实现，要通过碰撞测试
	m_nDir = Dir;
	if (Dir == DIR_UP)
	{
		m_rct.MoveToY(m_rct.top - m_nSpeed);
	}
	else if (Dir == DIR_DOWN)
	{
		m_rct.MoveToY(m_rct.top + m_nSpeed);
	}
	else if (Dir == DIR_LEFT)
	{
		m_rct.MoveToX(m_rct.left - m_nSpeed);
	}
	else if (Dir == DIR_RIGHT)
	{
		m_rct.MoveToX(m_rct.left + m_nSpeed);
	}
}

bool Object::IsOverLap(CRect& rct)
{
	CRect tmp;
	rct.NormalizeRect();
	return IntersectRect(tmp, rct, m_rct);
}

bool Object::IsOut()
{
	if (m_rct.bottom > Y_MAX || m_rct.top<0
		|| m_rct.right>X_MAX || m_rct.left < 0)
	{
		if (m_rct.bottom > Y_MAX)
			m_rct.OffsetRect(0, -(m_rct.bottom - Y_MAX + CORRECT));
		if (m_rct.top < 0)
			m_rct.OffsetRect(0, -m_rct.top + CORRECT);
		if (m_rct.right > X_MAX)
			m_rct.OffsetRect(-(m_rct.right - X_MAX + CORRECT), 0);
		if (m_rct.left < 0)
			m_rct.OffsetRect(-m_rct.left + CORRECT, 0);
		return true;
	}
	return false;
}