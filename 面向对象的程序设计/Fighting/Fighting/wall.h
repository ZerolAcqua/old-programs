#pragma once
#include "Object.h"

#define BRICK 0
#define METAL 1
#define GRASS 2
#define WATER 3
#define WOOD 4
#define BASE 5

class wall :public Object
{
public:
	wall(CPoint pt,int type);
	void Move(int Dir) {};
	virtual void disappear() {};
	void Show(CDC* pDC);

	//ǽ�����ͺ�����
	int type = 0;
	bool ShellPass = false;
	bool TankPass = false;
	bool destroyable = false;


	//��ͼ
	static CImage w_img[6];


	//�����õĹ��캯��
};

