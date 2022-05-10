#include "Gauss.h"
#include <math.h>
#ifndef EPSILON
#define EPSILON 1e-10
#endif // !EPSILON

#ifndef MAX_ITERATION
#define MAX_ITERATION 20
#endif // !MAX_ITERATION



int directGaussTrans(double ePrime2, double c, double B_1, double L_1, double A_12, double S, double* result)
{
	
	// 计算V,M_1,N_1
	double M_1 = 0, N_1 = 0, V_1 = 0;
	V_1 = sqrt(1 + ePrime2 * pow(cos(B_1), 2));
	M_1 = c / pow(V_1, 3);
	N_1 = c / V_1;
	
	// 迭代的变量
	double dB = 0, dL = 0, dA = 0;
	double dB_ = 0, dL_ = 0, dA_ = 0;
	double B_m = 0, L_m = 0, A_m = 0;
	double M_m = 0, N_m = 0, V_m = 0;
	double t_m = 0, t_m2 = 0, eta_m2 = 0, eta_m4 = 0;


	// 迭代初值(这里没有把弧度转换为秒)
	double dB_0 = 0, dL_0 = 0, dA_0 = 0;
	dB = dB_0 = S / M_1 * cos(A_12);
	dL = dL_0 = S / N_1 * sin(A_12) / cos(B_1);
	dA = dA_0 = dL * sin(B_1);

	// 循环体
	for (int i = 0; i < MAX_ITERATION; ++i)
	{
		B_m = B_1 + 0.5 * dB;
		A_m = A_12 + 0.5 * dA;
		t_m = tan(B_m);
		eta_m2 = ePrime2 * pow(cos(B_m), 2);
		eta_m4 = pow(eta_m2, 2);
		t_m2 = pow(t_m, 2);



		V_m = sqrt(1 + ePrime2 * pow(cos(B_m), 2));
		M_m = c / pow(V_m, 3);
		N_m = c / V_m;

		dB_ = S / M_m * cos(A_m) *
			(
				1 + pow(S, 2) / (24 * pow(N_m, 2)) *
				(
					pow(sin(A_m), 2) * (2 + 3 * t_m2 + 2 * t_m2 * eta_m2) +
					3 * eta_m2 * pow(cos(A_m), 2) * (-1 + t_m2 - eta_m2 - 4 * t_m2 * eta_m2)
				)
			);
		dL_ = S / N_m * sin(A_m) / cos(B_m) * 
			(
				1 + pow(S, 2) / (24 * pow(N_m, 2)) *
				(
					pow(sin(A_m), 2) * t_m2 - pow(cos(A_m), 2) * (1 + eta_m2 - 9 * t_m2 * eta_m2 + eta_m4)
				)
			);
		dA_ = S / N_m * sin(A_m) * t_m *
			(
				1 + pow(S, 2) / (24 * pow(N_m, 2)) *
				(
					pow(cos(A_m), 2) * (2 + 7 * eta_m2 + 9 * t_m2 * eta_m2 + 5 * eta_m4) +
					pow(sin(A_m), 2) * (2 + t_m2 + 2 * eta_m2)
				)
			);


		//// 距离小于70km时的简化公式
		//dB_ = S / M_m * cos(A_m) * (1 + pow(dL_0, 2) / 12 + pow(dA_0, 2) / 24);
		//dL_ = S / N_m * sin(A_m) / cos(B_m) * (1 + pow(dA_0, 2) / 24 - pow(dB_0, 2) / 24);
		//dA_ = S / N_m * sin(A_m) * tan(B_m) * (1 + pow(dB_0, 2) / 12 + pow(dL_0 * cos(B_m), 2) / 12 + pow(dA_0, 2) / 24);

		if (fabs(dB_ - dB) < EPSILON &&
			fabs(dL_ - dL) < EPSILON &&
			fabs(dA_ - dA) < EPSILON)
		{
			result[0] = B_1 + dB;
			result[1] = L_1 + dL;
			if (A_12 + dA > PI)
			{
				result[2] = A_12 + dA - PI;
			}
			else
			{
				result[2] = A_12 + dA + PI;
			}
			return 0;
		}
		dB = dB_;
		dL = dL_;
		dA = dA_;
	}


	return -1;
}
void reverseGaussTrans(double ePrime2, double c, double B_1, double L_1, double B_2, double L_2, double* result)
{
	// 不使用rou''转换为秒
	double B_m = (B_1 + B_2) / 2;
	double L_m = (L_1 + L_2) / 2;
	double dA_m = 0;

	double dB = B_2 - B_1;
	double dL = L_2 - L_1;
	double dA = 0;

	double V_m = sqrt(1 + ePrime2 * pow(cos(B_m), 2));
	double M_m = c / pow(V_m, 3);
	double N_m = c / V_m;

	double t_m = tan(B_m);
	double eta_m2 = ePrime2 * pow(cos(B_m), 2);
	double eta_m4 = pow(eta_m2, 2);
	double t_m2 = pow(t_m, 2);

	double U = 0;//U=SsinAm
	double V = 0;//V=ScosAm


	double r_01 = 0, r_21 = 0, r_03 = 0;
	double S_10 = 0, S_12 = 0, S_30 = 0;
	double t_01 = 0, t_21 = 0, t_03 = 0;

	r_01 = N_m * cos(B_m);
	r_21 = N_m * cos(B_m) / (24 * pow(V_m, 4)) * (1 + eta_m2 - 9 * eta_m2 * t_m2 + eta_m4);//
	r_03 = -N_m / 24 * pow(cos(B_m), 3) * t_m2;
	
	S_10 = N_m / pow(V_m, 2);
	S_12 = -N_m / (24 * pow(V_m, 2)) * pow(cos(B_m), 2) * (2 + 3 * t_m2 + 2 * eta_m2 );//
	S_30 = N_m / (8 * pow(V_m, 6)) * (eta_m2 - t_m2 * eta_m2);//

	t_01 = t_m * cos(B_m);
	t_21 = cos(B_m) * t_m / (24 * pow(V_m, 4)) * (2 + 7 * eta_m2 + 9 * t_m2 * eta_m2 + 5 * eta_m4);
	t_03 = pow(cos(B_m), 3) / 24 * t_m * (2 + t_m2 + 2 * eta_m2);

	U = r_01 * dL + r_21 * pow(dB, 2) * dL + r_03 * pow(dL, 3);
	V = S_10 * dB + S_12 * dB * pow(dL, 2) + S_30 * pow(dB, 3);
	dA = t_01 * dL + t_21 * pow(dB, 2) * dL + t_03 * pow(dL, 3);
	
	double A_m = 0;
	double T = 0, C = fabs(V / U);

	if (fabs(dB) >= fabs(dL))
	{
		T = atan(fabs(U / V));
	}
	else
	{
		T = PI / 4 + atan((1 - C) / (1 + C));
	}

	if (dB > 0 && dL >= 0)
	{
		A_m = T;
	}
	else if (dB < 0, dL >= 0)
	{
		A_m = PI - T;
	}
	else if (dB <= 0, dL < 0)
	{
		A_m = PI + T;
	}
	else if (dB > 0, dL < 0)
	{
		A_m = 2 * PI - T;
	}
	else
	{
		A_m = PI / 2;
	}

	result[0] = A_m - 0.5 * dA;			// A_12
	result[1] = A_m + 0.5 * dA - PI;	// A_21 
	if (result[1] < 0)
		result[1] += 2 * PI;
	result[2] = U / sin(A_m);			// S
}