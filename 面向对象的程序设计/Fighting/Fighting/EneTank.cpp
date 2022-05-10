#include "pch.h"
#include "EneTank.h"


//静态成员变量初始化
CImage EneTank::ene_img[4];

//消灭坦克数
int EneTank::total_num = 0;
//难度
int EneTank::level = 1;

EneTank::EneTank()
{
	m_firetime = m_time = GetTickCount();
	m_nSpeed = 1+level;
	m_nDir = 1;
	m_life = 1;

	//随机方向和持续时间
	unsigned int i;
	rand_s(&i);
	m_rct = CRect(birth[total_num % 3],CSize(Tank_size,Tank_size));
	rand_s(&i);
	m_span = 1 + i % 2;
	m_firespan = 5 + i % 2;
}

EneTank::EneTank(int pos)
{
	m_firetime = m_time = GetTickCount();
	m_nSpeed = 1+level;
	m_nDir = 1;
	m_life = 1;
	//随机运动时间和开火间隔
	unsigned int i;
	rand_s(&i);
	m_rct = CRect(birth[pos%3], CSize(Tank_size, Tank_size));
	rand_s(&i);
	m_span = 1 + i % 2;
	m_firespan = 2 + (i % 12) / level;
}

void EneTank::re_time()
{
	m_span = 0.5;
}

bool EneTank::fireClock()
{
	//原理和AutoMove相似，随机开炮时间间隔
	if ((GetTickCount() - m_firetime) / 1000 < m_firespan)
		return false;
	else
	{
		unsigned int i;
		rand_s(&i);
		m_firetime = GetTickCount();
		m_firespan = 1+(i % 6) / (0.5*level);
		return true;
	}
}

void EneTank::Show(CDC* pDC)
{
	if (m_nDir == DIR_UP)
		ene_img[DIR_UP].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_DOWN)
		ene_img[DIR_DOWN].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_LEFT)
		ene_img[DIR_LEFT].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
	else if (m_nDir == DIR_RIGHT)
		ene_img[DIR_RIGHT].Draw(pDC->GetSafeHdc(), m_rct, Gdiplus::InterpolationModeHighQuality);
}



void EneTank::AutoMove()
{
	////随机部分（后期实现时可以不用，改为遇墙再随机即可）
	if((GetTickCount()-m_time)/1000.0<m_span )
		Move(m_nDir);
	else//否则重新随机方向和持续时间
		ChangeDir();
}

void EneTank::ChangeDir()
{
	unsigned int i;
	rand_s(&i);
	m_nDir = (i % 6);//方向,可调节向下运动的权重
	if (m_nDir > 3)
		m_nDir = 1;
	rand_s(&i);
	m_nSpeed = 1 + level;
	m_span = 3 + i % 5;//持续时间
	m_time = GetTickCount();
}

void EneTank::eme_down()
{
	total_num++;
}