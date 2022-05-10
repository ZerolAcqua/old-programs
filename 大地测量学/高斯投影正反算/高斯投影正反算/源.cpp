#define _CRT_SECURE_NO_WARNINGS 1

#include "stdio.h"
#include <iostream>
#include <iomanip>
#include <math.h>

#include "GaussCoodTrans.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif // !PI
#ifndef ROUPP
#define ROUPP 206264.806247096355
#endif // !ROUPP

using namespace::std;

int main()
{
	double X_0 = 0;
	double Y_0 = 0;
	double L_0 = 0;

	// 读取param.txt
	FILE* fin = fopen("param.txt", "r");
	if (fin == NULL) {
		perror("打开文件时发生错误");
		return -1;
	}
	char* data = new char[512];

	fgets(data, 512, fin);
	fgets(data, 512, fin);
	sscanf(data, "%lf %lf %lf", &X_0, &Y_0, &L_0);

	fclose(fin);

	double B = 0;
	double L = 0;





	// 正算
	int n = 0;
	fin = fopen("data4DirectTrans.txt", "r");
	if (fin == NULL) {
		perror("打开文件时发生错误");
		return -1;
	}

	FILE* fout = fopen("directResult.txt", "w");
	char ID[64] = { 0 };
	double result[2] = { 0 };

	fgets(data, 512, fin);
	sscanf(data, " %d ", &n);
	fprintf(fout, "%d\n", n);

	cout << "高斯投影坐标正算" << endl;
	for (int i = 0; i < n; ++i)
	{
		fgets(data, 512, fin);
		sscanf(data, "%s %lf %lf ", ID, &B, &L);
		directGaussCoodTrans(B * 3600 / ROUPP, L * 3600 / ROUPP, L_0 * 3600 / ROUPP, X_0, Y_0, result);
		cout << setprecision(17) << ID << endl
			<< "X: " << result[0] << "m" << endl
			<< "Y: " << result[1] << "m" << endl;
		fprintf(fout, "%s %f %f\n", ID, result[0], result[1]);
	}
	cout << endl;
	fclose(fout);
	fclose(fin);

	
	// 反算
	double result2[2] = { 0 };
	double X = 0, Y = 0;

	fin = fopen("directResult.txt", "r");
	if (fin == NULL) {
		perror("打开文件时发生错误");
		return -1;
	}
	fout = fopen("reverseResult.txt", "w");
	
	fgets(data, 512, fin);
	sscanf(data, " %d ", &n);
	fprintf(fout, "%d\n", n);


	cout << "高斯投影坐标反算" << endl;

	for (int i = 0; i < n; ++i)
	{
		fgets(data, 512, fin);
		sscanf(data, "%s %lf %lf ", ID, &X, &Y);
		reverseGaussCoodTrans(X, Y, L_0 * 3600 / ROUPP, X_0, Y_0, result2);
		cout << setprecision(17) << ID << endl
			<< "B: " << result2[0] * ROUPP / 3600 << "°" << endl
			<< "L: " << result2[1] * ROUPP / 3600 << "°" << endl;
		fprintf(fout, "%s %f %f\n", ID, result2[0] * ROUPP / 3600, result2[1] * ROUPP / 3600);
	}
	fclose(fout);
	fclose(fin);

	delete[]data;
}