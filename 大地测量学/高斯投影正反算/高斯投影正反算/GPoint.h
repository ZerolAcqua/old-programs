#pragma once
#include "stdio.h"
#include "string"

using namespace::std;


class GPoint
{
public:

	// ---- ��Ա���� ----
	string msID = "";
	// X Y
	double mdCood[2] = { 0 };

	// ---- �������������� ----

	/**
	*	@brief ���캯��
	*
	*/
	explicit GPoint();

	/**
	*	@brief ���캯��
	*
	*	@Param ID	���ID
	*	@Param cood �����ά����
	*/
	explicit GPoint(char* ID, double* cood);
	~GPoint() { };

	// ---- �޸�����ʺ��� ----

	/**
	*	@brief �޸ĺ���
	*
	*	���õ��ID������
	*	@Param ID	���ID
	*	@Param cood �����ά����
	*/
	void SetPointIdCood(const char* ID, double* cood);

	// ---- ���ܺ��� ----

	/**
	*	@brief ���ܺ���
	*
	*	��������֮��ľ���
	*	@Param point �������ĵ�
	*	@return ���������ľ���
	*/
	double GetDist(GPoint point);

	///*----------------------���������---------------------*/
	// == �е�

	/**
	*	@brief �����==����
	*
	*	������������ͬ���е�
	*	��������㹻�ӽ���Ҳ����Ϊ��ͬ
	*	��ֵΪ0.1mm
	*/
	bool operator==(GPoint tmp);
};

