#pragma once
#include "Tank.h"
class EneTank :	public Tank
{
public:
	EneTank();
	EneTank(int pos);
	void Show(CDC* pDC);
	void AutoMove();
	void EneTank::ChangeDir();
	void re_time();
	bool fireClock();
	static void eme_down();

	DWORD m_time;
	DWORD m_firetime;
	double m_span;
	int m_firespan;


	//消灭坦克的数量,是静态成员变量
	static int total_num;
	static int level;
	//贴图
	static CImage ene_img[4];
protected:
	//固定的出生点,静态成员变量
	static CPoint birth[3];
	
};

