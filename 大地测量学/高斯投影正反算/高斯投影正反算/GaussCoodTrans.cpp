#include "GaussCoodTrans.h"
#include <math.h>


#ifndef ROUPP
#define ROUPP 206264.806247096355
#endif // !ROUPP

void directGaussCoodTrans(double B, double L, double L_0, double X_0, double Y_0, double* result)
{
	// 克拉索夫斯基椭球参数
	const double a = 6378245;
	const double b = 6356863.0187730473;
	const double c = 6399698.9017827110;
	const double alpha = 1 / 298.3;
	const double e2 = 0.006693421622966;
	const double ep2 = 0.006738525414683;

	double cos_B_sq = pow(cos(B), 2);

	double l = L - L_0;
	double l_sq = pow(l, 2);
	double N = 6399698.902 - (21562.267 - (108.973 - 0.612 * cos_B_sq) * cos_B_sq) * cos_B_sq;
	double a_0 = 32140.404 - (135.3302 - (0.7092 - 0.004 * cos_B_sq) * cos_B_sq) * cos_B_sq;
	double a_4 = (0.25 + 0.00252 * cos_B_sq) * cos_B_sq - 0.04166;
	double a_6 = (0.166 * cos_B_sq - 0.084) * cos_B_sq;
	double a_3 = (0.3333333 + 0.001123 * cos_B_sq)* cos_B_sq - 0.1666667;
	double a_5 = 0.0083 - (0.1667 - (0.1968 + 0.004 * cos_B_sq) * cos_B_sq) * cos_B_sq;
	
	result[0] = 6367558.4969 * B - (a_0 - (0.5 + (a_4 + a_6 * l_sq) * l_sq) * l_sq * N) * sin(B) * cos(B) + X_0;
	result[1] = (1 + (a_3 + a_5 * l_sq) * l_sq) * l * N * cos(B);

}

void reverseGaussCoodTrans(double X, double Y, double L_0, double X_0, double Y_0, double* result)
{
	double beta = X / 6367558.4969;
	double cos_beta_sq = pow(cos(beta), 2);
	double B_f = beta + (50221746 + (293622 + (2350 + 22 * cos_beta_sq) * cos_beta_sq) * cos_beta_sq) * sin(beta) * cos(beta) * 1e-10;
	double cos_B_f_sq = pow(cos(B_f), 2);

	double N_f = 6399698.902 - (21562.267 - (108.973 - 0.612 * cos_B_f_sq) * cos_B_f_sq) * cos_B_f_sq;
	double Z = Y / (N_f * cos(B_f));

	double b_2 = (0.5 + 0.003369 * cos_B_f_sq) * sin(B_f) * cos(B_f);
	double b_3 = 0.333333 - (0.166667 - 0.001123 * cos_B_f_sq) * cos_B_f_sq;
	double b_4 = 0.25 + (0.16161 + 0.00562 * cos_B_f_sq) * cos_B_f_sq;
	double b_5 = 0.2 - (0.1667 - 0.0088 * cos_B_f_sq) * cos_B_f_sq;

	double l = (1 - (b_3 - b_5 * pow(Z, 2)) * pow(Z, 2)) * Z;
	result[0] = B_f - (1 - (b_4 - 0.12 * pow(Z, 2)) * pow(Z, 2)) * pow(Z, 2) * b_2;
	result[1] = L_0 + l;
}