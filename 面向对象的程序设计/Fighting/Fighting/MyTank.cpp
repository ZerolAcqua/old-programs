#include "pch.h"
#include "MyTank.h"
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")


CImage MyTank::m_img[4];

Shell MyTank::Fire()
{
	Shell sh;
	CPoint pt = m_rct.CenterPoint();
	sh.Create(pt, m_nDir, m_nSpeed*40);

	//²¥·ÅÉùÒô
	CString strFileName;
	TCHAR cPath[1024];
	GetModuleFileName(NULL, cPath, 1024);
	strFileName = cPath;
	CString strSound = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"fire.wav";
	PlaySound(strSound, 0, SND_FILENAME | SND_ASYNC);

	return sh;
}


void MyTank::Show(CDC* pDC)
{
	//ÏÔÊ¾Ì¹¿Ë
	if (m_nDir == DIR_UP)
		m_img[DIR_UP].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_DOWN)
		m_img[DIR_DOWN].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_LEFT)
		m_img[DIR_LEFT].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_RIGHT)
		m_img[DIR_RIGHT].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
}

MyTank::MyTank()
{
	m_respanpos = CPoint(150, 450);
	m_rct = CRect(m_respanpos, CSize(Tank_size,Tank_size));
	m_life = 5;
	m_nSpeed = 4;
	m_nDir = 0;
}
bool MyTank::show_life(CDC* pDC,CRect rct)
{
	for (int i = 1; i <= m_life; i++)
	{
		if (i % 5)
		{
			m_img[DIR_UP].Draw(pDC->GetSafeHdc(), rct, Gdiplus::InterpolationModeHighQuality);
			rct.OffsetRect(rct.Width() + 2, 0);
		}
		else
		{
			m_img[DIR_UP].Draw(pDC->GetSafeHdc(), rct, Gdiplus::InterpolationModeHighQuality);
			rct.OffsetRect(-(rct.Width() + 2)*4, rct.Height() + 2);
		}

	}
	return true;
}

bool MyTank::respan()
{
	m_rct = CRect(m_respanpos, CSize(Tank_size, Tank_size));
	m_nDir = 0;
	return true;
}

bool MyTank::gameover()
{
	this->m_life = 0;
	return true;
}

void MyTank::setDir(int i)
{
	if (i > 4)
	{
		m_move = false;
	}
	else
	{
		m_nDir = i;
		m_move = true;
	}
}

void MyTank::myMove()
{
	if (m_move)
	{
		Move(m_nDir);
	}
}