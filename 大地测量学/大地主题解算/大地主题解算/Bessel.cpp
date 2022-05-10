#include "Bessel.h"
#include <math.h>

#ifndef EPSILON
#define EPSILON 1e-10
#endif // !EPSILON

#ifndef MAX_ITERATION
#define MAX_ITERATION 20
#endif // !MAX_ITERATION


void directBesselTrans( double B_1, double L_1, double A_12, double S, double* result)
{
	// 克拉索夫斯基椭球参数
	const double e2 = 0.006693421622966;
	const double c = 6399698.9017827110;


	double W_1 = sqrt(1 - e2 * pow(sin(B_1), 2));
	double sin_u_1 = sin(B_1) * sqrt(1 - e2) / W_1;
	double cos_u_1 = cos(B_1) / W_1;

	double sin_A_12 = sin(A_12);
	double cos_A_12 = cos(A_12);

	double sin_A_0 = cos_u_1 * sin_A_12;
	double cot_sigma_1 = cos_u_1 * cos_A_12 / sin_u_1;
	double sin_2sigma_1 = 2 * cot_sigma_1 / (pow(cot_sigma_1, 2) + 1);
	double cos_2sigma_1 = (pow(cot_sigma_1, 2) - 1) / (pow(cot_sigma_1, 2) + 1);
	double cos_A_0_sq = 1 - pow(sin_A_0, 2);

	// 克拉索夫斯基椭球
	double A = 6356863.020 + (10708.949 - 13.474 * cos_A_0_sq) * cos_A_0_sq;
	double B = (5354.469 - 8.978 * cos_A_0_sq) * cos_A_0_sq;
	double C = (2.238 * cos_A_0_sq) * cos_A_0_sq + 0.006;
	double alpha = (691.46768 - (0.58143 - 0.00144 * cos_A_0_sq) * cos_A_0_sq) / ROUPP;
	double beta = (0.2907 - 0.0010 * cos_A_0_sq) * cos_A_0_sq / ROUPP;


	double sigma_0 = (S - (B + C * cos_2sigma_1)*sin_2sigma_1) / A;
	//sin2(sigma1+sigma0)
	double sin_2ss = sin_2sigma_1 * cos(2 * sigma_0) + cos_2sigma_1 * sin(2 * sigma_0);
	//cos2(sigma1+sigma0)
	double cos_2ss = cos_2sigma_1 * cos(2 * sigma_0) - sin_2sigma_1 * sin(2 * sigma_0);
	double sigma = sigma_0 + (B + 5 * C * cos_2ss) * sin_2ss / A;

	//经度改正数
	double delta = (alpha * sigma + beta * (sin_2ss - sin_2sigma_1)) * sin_A_0;

	double sin_sigma = sin(sigma);
	double cos_sigma = cos(sigma);


	double sin_u_2 = sin_u_1 * cos_sigma + cos_u_1 * cos_A_12 * sin_sigma;
	double B_2 = atan(sin_u_2 / sqrt(1 - e2) / sqrt(1 - pow(sin_u_2, 2)));
	double lambda = atan(
		sin_A_12 * sin_sigma
		/ (cos_u_1 * cos_sigma - sin_u_1 * sin_sigma * cos_A_12)
	);


	// 符号判断
	if (sin_A_12 > 0 && (tan(lambda) > 0))
	{
		lambda = fabs(lambda);
	}
	else if (sin_A_12 > 0 && (tan(lambda) < 0))
	{
		lambda = PI - fabs(lambda);
	}
	else if (sin_A_12 < 0 && (tan(lambda) < 0))
	{
		lambda = -fabs(lambda);
	}
	else if (sin_A_12 < 0 && (tan(lambda) > 0))
	{
		lambda = fabs(lambda) - PI;
	}

	double L_2 = L_1 + lambda - delta;		//???????????????????????????
	double A_21 = atan(
		cos_u_1 * sin_A_12
		/ (cos_u_1 * cos_sigma * cos_A_12 - sin_u_1 * sin_sigma)
	);



	// 符号判断
	if (sin_A_12 < 0 && (tan(A_21) > 0))
	{
		A_21 = fabs(A_21);
	}
	else if (sin_A_12 < 0 && (tan(A_21) < 0))
	{
		A_21 = PI - fabs(A_21);
	}
	else if (sin_A_12 > 0 && (tan(A_21) > 0))
	{
		A_21 = PI + fabs(A_21);
	}
	else if (sin_A_12 > 0 && (tan(A_21) < 0))
	{
		A_21 = 2 * PI - fabs(A_21);
	}

	result[0] = B_2;
	result[1] = L_2;
	result[2] = A_21;


	return;
}
int reverseBesselTrans(double B_1, double L_1, double B_2, double L_2, double* result)
{
	// 克拉索夫斯基椭球参数
	const double e2 = 0.006693421622966;
	const double c = 6399698.9017827110;

	double W_1 = sqrt(1 - e2 * pow(sin(B_1), 2));
	double sin_u_1 = sin(B_1) * sqrt(1 - e2) / W_1;
	double cos_u_1 = cos(B_1) / W_1;
	double W_2 = sqrt(1 - e2 * pow(sin(B_2), 2));
	double sin_u_2 = sin(B_2) * sqrt(1 - e2) / W_2;
	double cos_u_2 = cos(B_2) / W_2;
	double L = L_2 - L_1;

	double a_1 = sin_u_1 * sin_u_2;
	double a_2 = cos_u_1 * cos_u_2;
	double b_1 = cos_u_1 * sin_u_2;
	double b_2 = sin_u_1 * cos_u_2;

	// 迭代
	double delta = 0;
	double delta_ = 0;
	double lambda = 0;
	double p = 0;
	double q = 0;
	double A_1 = 0;
	
	double sin_sigma = 0;
	double cos_sigma = 0;
	double sigma = 0;
	double sin_A_0 = 0;
	double cos_A_0_sq = 0;
	double x = 0;

	double alpha = 0;
	double beta_p = 0;

	for (int i = 0; i < MAX_ITERATION; ++i)
	{
		lambda = L + delta;
		p = cos_u_2 * sin(lambda);
		q = b_1 - b_2 * cos(lambda);
		A_1 = atan(p / q);

		if (p > 0 && q > 0)
		{
			A_1 = fabs(A_1);
		}
		else if (p > 0 && q < 0)
		{
			A_1 = PI - fabs(A_1);
		}
		else if (p < 0 && q < 0)
		{
			A_1 = PI + fabs(A_1);
		}
		else if (p < 0 && q > 0)
		{
			A_1 = 2 * PI - fabs(A_1);
		}



		sin_sigma = p * sin(A_1) + q * cos(A_1);
		cos_sigma = a_1 + a_2 * cos(lambda);
		sigma = atan(sin_sigma / cos_sigma);

		if (cos_sigma > 0)
		{
			sigma = fabs(sigma);
		}
		else if (cos_sigma < 0)
		{
			sigma = PI - fabs(sigma);
		}


		sin_A_0 = cos_u_1 * sin(A_1);
		cos_A_0_sq = 1 - pow(sin_A_0, 2);

		// ??????
		alpha = (33523299 - (28189 - 70 * cos_A_0_sq) * cos_A_0_sq) * 1e-10 ;
		beta_p = (28189 - 94 * cos_A_0_sq) * 1e-10 ;


		x = 2 * a_1 - cos_A_0_sq * cos_sigma;
		delta_ = (alpha * sigma - beta_p * x * sin_sigma) * sin_A_0;



		if (fabs(delta_-delta) < EPSILON)
		{
			double A = 6356863.020 + (10708.949 - 13.474 * cos_A_0_sq) * cos_A_0_sq;
			double B_pp = 10708.938 - 17.956 * cos_A_0_sq;
			double C_pp = 4.487;
			double y = (pow(cos_A_0_sq, 2) - 2 * pow(x, 2)) * cos_sigma;


			// lambda??????
			double p_ = cos_u_1 * sin(lambda);
			double q_ = b_1 * cos(lambda) - b_2;

			double S = A * sigma + (B_pp * x + C_pp * y) * sin_sigma;
			double A_2 = atan(p_ / q_);


			if (p_ > 0 && q_ > 0)
			{
				A_2 = PI + fabs(A_2);
			}
			else if (p_ > 0 && q_ < 0)
			{
				A_2 = 2 * PI - fabs(A_2);
			}
			else if (p_ < 0 && q_ < 0)
			{
				A_2 = fabs(A_2);
			}
			else if (p_ < 0 && q_ > 0)
			{
				A_2 = PI - fabs(A_2);
			}



			result[0] = A_1;
			result[1] = A_2;
			result[2] = S;

			return 0;
		}
		delta = delta_;
	}
	return -1;
}


