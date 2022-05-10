#pragma once
#include "stdio.h"
#include "string"

using namespace::std;


class GPoint
{
public:

	// ---- 成员变量 ----
	string msID = "";
	// X Y
	double mdCood[2] = { 0 };

	// ---- 构造与析构函数 ----

	/**
	*	@brief 构造函数
	*
	*/
	explicit GPoint();

	/**
	*	@brief 构造函数
	*
	*	@Param ID	点的ID
	*	@Param cood 点的三维坐标
	*/
	explicit GPoint(char* ID, double* cood);
	~GPoint() { };

	// ---- 修改与访问函数 ----

	/**
	*	@brief 修改函数
	*
	*	设置点的ID和坐标
	*	@Param ID	点的ID
	*	@Param cood 点的三维坐标
	*/
	void SetPointIdCood(const char* ID, double* cood);

	// ---- 功能函数 ----

	/**
	*	@brief 功能函数
	*
	*	计算两点之间的距离
	*	@Param point 待求距离的点
	*	@return 返回两点间的距离
	*/
	double GetDist(GPoint point);

	///*----------------------运算符重载---------------------*/
	// == 判等

	/**
	*	@brief 运算符==重载
	*
	*	如果两点点名相同则判等
	*	如果两点足够接近，也可认为相同
	*	阈值为0.1mm
	*/
	bool operator==(GPoint tmp);
};

