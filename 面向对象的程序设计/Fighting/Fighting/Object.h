#pragma once
#define CORRECT 0


//��λ���ӵĴ�С
//const CSize unitSize(50, 50);
/*��һ��ط��������⣬
��֪��Ϊʲômytank���ղ���unitsize
��������Ϊ�����ʱ��unitSize��δ��ʼ��*/

//�����Ӧ������
const int DIR_UP = 0;
const int DIR_DOWN = 1;
const int DIR_LEFT = 2;
const int DIR_RIGHT = 3;
//��Ԫ���ӵĴ�С
const int size = 50;
const int Tank_size = 45;
//��Χ
const int X_MAX = 600;
const int Y_MAX = 500;
//+20��Ϊ�˷�ֹ�����жϵ��ӳٳ���ͼƬ����
//�����������Գ��Լӿ��ж��ٶ�
const CRect Range(CPoint(0, 0), CPoint(X_MAX + 200, Y_MAX + 40));

class Object
{
public:
	virtual void Show(CDC* pDC) {};
	void Move(int Dir);

	void SetPos(CRect rct) { m_rct = rct; m_rct.NormalizeRect(); };
	CRect GetPos() { return m_rct; };
	void setSpeed(int n) { this->m_nSpeed = n;}

	bool m_movable = true;	
	bool IsOverLap(CRect& rct);//�ص�����ײ���Ի���
	bool IsOut();

	CRect m_rct;

protected:
	int m_nDir;//0-3:up\down\left\right
	int m_nSpeed;	
};



