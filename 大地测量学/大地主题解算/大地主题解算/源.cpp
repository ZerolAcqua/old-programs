#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "Gauss.h"
#include "Bessel.h"

#define BJ54
#ifdef BJ54
#define A 6378245
#define B 6356863.0187730473
#define C 6399698.9017827110
#define ALPHA 1/298.3
#define E_SQUARE	   0.006693421622966
#define E_PRIME_SQUARE 0.006738525414683
#endif  

using namespace::std;

int main()
{
	// -------- 高斯平均引数法 --------
	// 正算
	double B_1 =0;
	double L_1 = 0;
	double A_12 = 0;
	double S = 0;

	B_1 = (47.4652647 * 3600) / ROUPP;
	L_1 = (35.493633 * 3600) / ROUPP;
	A_12 = (44.1213664 * 3600) / ROUPP;
	S = 44797.2826;
	double result[3] = { 0 };
	directGaussTrans(E_PRIME_SQUARE, C, B_1, L_1, A_12, S, result);
	cout << "高斯引数法正算" << setprecision(17) << endl
		<< "B2: " << result[0] * ROUPP / 3600 <<"°"<< endl
		<< "L2: " << result[1] * ROUPP / 3600 << "°" << endl
		<< "A21: " << result[2] * ROUPP / 3600 << "°" << endl;
	// 反算
	double result2[3] = { 0 };
	reverseGaussTrans(E_PRIME_SQUARE, C, B_1, L_1, result[0], result[1], result2);
	cout << "高斯引数法反算" << setprecision(17) << endl
		<< "A12: " << result2[0] * ROUPP / 3600 << "°" << endl
		<< "A21: " << result2[1] * ROUPP / 3600 << "°" << endl
		<< "S: " << result2[2] << endl;
	cout << "--------------------------------------" << endl;


	// -------- 白塞尔法 --------
	// 测试数据
	// 正算
	B_1 = (47 * 3600 + 46 * 60 + 52.6470) / ROUPP;
	L_1 = (35 * 3600 + 49 * 60 + 36.3300) / ROUPP;
	A_12 = (44 * 3600 + 12 * 60 + 13.664) / ROUPP;
	S = 44797.2826;

	directBesselTrans(B_1, L_1, A_12, S, result);
	cout << "白塞尔法正算" << setprecision(17) << endl
		<< "B2: " << result[0] * ROUPP / 3600 << "°" << endl
		<< "L2: " << result[1] * ROUPP / 3600 << "°" << endl
		<< "A21: " << result[2] * ROUPP / 3600 << "°" << endl;
	// 反算，结果似乎只有小数点后6位的精度
	reverseBesselTrans(B_1, L_1, result[0], result[1], result2);
	cout << "白塞尔法反算" << setprecision(17) << endl
		<< "A12: " << result2[0] * ROUPP / 3600 << "°" << endl
		<< "A21: " << result2[1] * ROUPP / 3600 << "°" << endl
		<< "S: " << result2[2] << endl;


	cout << 47 + 46 / 60 + 52.6470 / 3600 << endl;
	cout << 35 + 49 / 60 + 36.3300 / 3600 << endl;
	cout << 44 + 12 / 60 + 13.664 / 3600 << endl;
}


