#pragma once

/**
*	@brief 正算公式
*	求解   X Y		
*
*	@param B	纬度				使用弧度
*	@param L	经度				使用弧度
*  	@param L_0	中央经度			使用弧度
*	@param X_0	坐标原点常数		使用 m 单位
*	@param Y_0	坐标原点常数		使用 m 单位
*	@param result 存储计算的结果（ X , Y ）使用 m 单位
*/
void directGaussCoodTrans(double B, double L, double L_0, double X_0, double Y_0, double* result);

/**
*	@brief 反算公式
*	 求解   B L					使用弧度
*
*	@param X	高斯坐标系X坐标		使用 m 单位	
*	@param Y	高斯坐标系Y坐标		使用 m 单位
*  	@param L_0	中央经度			使用弧度
*	@param X_0	坐标原点常数		使用 m 单位
*	@param Y_0	坐标原点常数		使用 m 单位
*	@param result 存储计算的结果（ B , L ）使用 m 单位
*/
void reverseGaussCoodTrans(double X, double Y, double L_0, double X_0, double Y_0, double* result);
