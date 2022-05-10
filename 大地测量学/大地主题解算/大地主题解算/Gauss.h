#pragma once
#ifndef PI
#define PI 3.14159265358979323846
#endif // !PI
#ifndef ROUPP
#define ROUPP 206264.806247096355
#endif // !ROUPP


/**
*	@brief 正算公式
*	求解   B_2  L_2  A_21   使用弧度
*
*	@param ePrime2 e'^2
*	@param c
*  	@param B_1 使用弧度单位
*	@param L_1 使用弧度单位
*	@param A_12 使用弧度单位
*	@param S
*	@param result 存储计算的结果（ B_2  L_2  A_21 ）使用弧度单位
*	@return 迭代成功返回0，迭代失败返回-1
*/
int directGaussTrans(double ePrime2, double c, double B_1, double L_1, double A_12, double S, double* result);

/**
*	@brief 反算公式
*	 求解   A_12  A_21  S   使用弧度
* 
*	@param ePrime2 e'^2
*	@param c
*  	@param B_1 使用弧度单位
*	@param L_1 使用弧度单位
*  	@param B_2 使用弧度单位
*	@param L_2 使用弧度单位
*	@param result 存储计算的结果（  A_12  A_21  S ）使用弧度单位
*/
void reverseGaussTrans(double ePrime2, double c, double B_1, double L_1, double B_2, double L_2, double* result);

