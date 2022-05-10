#include "pch.h"
#include "Tank.h"


Tank::Tank() 
{	
	m_nDir = 3;
	m_nSpeed = 2;
}



Shell Tank::fire()
{
	Shell sh;
	CPoint pt = m_rct.CenterPoint();
	sh.Create(pt, m_nDir, m_nSpeed * 50);
	return sh;
}

bool Tank::TOisOverlap(Object& ob)
{
	CRect& rct = ob.m_rct;
	bool check = false;
	CRect tmp;
	rct.NormalizeRect();
	check = IntersectRect(tmp, rct, m_rct);
	if (check)
	{
		if (tmp.Height() < tmp.Width())
		{
			if (tmp.bottom < rct.bottom)
			{
				this->m_rct.OffsetRect(0, -(tmp.Height() - CORRECT));
				if (ob.m_movable)
				{
					rct.OffsetRect(0, (tmp.Height() - CORRECT));
				}
			}
			else
			{
				this->m_rct.OffsetRect(0, (tmp.Height() - CORRECT));
				if (ob.m_movable)
				{
					rct.OffsetRect(0, -(tmp.Height() - CORRECT));
				}
			}
		}
		else
		{
			if (tmp.right < rct.right)
			{
				this->m_rct.OffsetRect(-tmp.Width() - CORRECT, 0);
				if (ob.m_movable)
				{
					rct.OffsetRect(tmp.Width() + CORRECT, 0);
				}
			}
			else
			{
				this->m_rct.OffsetRect(tmp.Width() - CORRECT, 0);
				if (ob.m_movable)
				{
					rct.OffsetRect(-(tmp.Width() - CORRECT), 0);
				}
			}
		}
	}
	return check;
}

bool Tank::TisOverlap(CRect rct)
{
	bool check = false;
	CRect tmp;
	rct.NormalizeRect();
	check = IntersectRect(tmp, rct, m_rct);
	if (check)
	{
		if (tmp.Height() < tmp.Width())
		{
			if (tmp.bottom < rct.bottom)
			{
				this->m_rct.OffsetRect(0, -(tmp.Height()-CORRECT));
			}
			else
			{
				this->m_rct.OffsetRect(0, (tmp.Height()-CORRECT));
			}
		}
		else
		{
			if (tmp.right < rct.right)
			{
				this->m_rct.OffsetRect(-tmp.Width()-CORRECT,0);
			}
			else
			{
				this->m_rct.OffsetRect(tmp.Width()-CORRECT,0);
			}
		}
	}
	return check;
}



void Tank::Show(CDC* pDC)
{


	//pDC->Rectangle(m_rct);
	//pDC->MoveTo(m_rct.CenterPoint());
	//if (m_nDir == 0)
	//	pDC->LineTo(m_rct.CenterPoint().x, m_rct.CenterPoint().y - 20);
	//else if (m_nDir == 1)
	//	pDC->LineTo(m_rct.CenterPoint().x, m_rct.CenterPoint().y + 20);
	//else if (m_nDir == 2)
	//	pDC->LineTo(m_rct.CenterPoint().x - 20, m_rct.CenterPoint().y);
	//else if (m_nDir == 3)
	//	pDC->LineTo(m_rct.CenterPoint().x + 20, m_rct.CenterPoint().y);

//////////////////ÌùÍ¼µÄÊµÏÖ/////////////////////

	/*pDC->Rectangle(m_rct);
	pDC->MoveTo(m_rct.CenterPoint());*/

}


