#pragma once
#ifndef PI
#define PI 3.14159265358979323846
#endif // !PI
#ifndef ROUPP
#define ROUPP 206264.806247096355
#endif // !ROUPP


/**
*	@brief ���㹫ʽ
*	���   B_2  L_2  A_21   ʹ�û���
*
*	@param ePrime2 e'^2
*	@param c
*  	@param B_1 ʹ�û��ȵ�λ
*	@param L_1 ʹ�û��ȵ�λ
*	@param A_12 ʹ�û��ȵ�λ
*	@param S
*	@param result �洢����Ľ���� B_2  L_2  A_21 ��ʹ�û��ȵ�λ
*	@return �����ɹ�����0������ʧ�ܷ���-1
*/
int directGaussTrans(double ePrime2, double c, double B_1, double L_1, double A_12, double S, double* result);

/**
*	@brief ���㹫ʽ
*	 ���   A_12  A_21  S   ʹ�û���
* 
*	@param ePrime2 e'^2
*	@param c
*  	@param B_1 ʹ�û��ȵ�λ
*	@param L_1 ʹ�û��ȵ�λ
*  	@param B_2 ʹ�û��ȵ�λ
*	@param L_2 ʹ�û��ȵ�λ
*	@param result �洢����Ľ����  A_12  A_21  S ��ʹ�û��ȵ�λ
*/
void reverseGaussTrans(double ePrime2, double c, double B_1, double L_1, double B_2, double L_2, double* result);

