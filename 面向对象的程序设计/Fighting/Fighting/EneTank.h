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


	//����̹�˵�����,�Ǿ�̬��Ա����
	static int total_num;
	static int level;
	//��ͼ
	static CImage ene_img[4];
protected:
	//�̶��ĳ�����,��̬��Ա����
	static CPoint birth[3];
	
};

