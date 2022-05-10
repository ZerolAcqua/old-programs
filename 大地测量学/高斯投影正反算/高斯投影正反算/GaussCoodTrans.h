#pragma once

/**
*	@brief ���㹫ʽ
*	���   X Y		
*
*	@param B	γ��				ʹ�û���
*	@param L	����				ʹ�û���
*  	@param L_0	���뾭��			ʹ�û���
*	@param X_0	����ԭ�㳣��		ʹ�� m ��λ
*	@param Y_0	����ԭ�㳣��		ʹ�� m ��λ
*	@param result �洢����Ľ���� X , Y ��ʹ�� m ��λ
*/
void directGaussCoodTrans(double B, double L, double L_0, double X_0, double Y_0, double* result);

/**
*	@brief ���㹫ʽ
*	 ���   B L					ʹ�û���
*
*	@param X	��˹����ϵX����		ʹ�� m ��λ	
*	@param Y	��˹����ϵY����		ʹ�� m ��λ
*  	@param L_0	���뾭��			ʹ�û���
*	@param X_0	����ԭ�㳣��		ʹ�� m ��λ
*	@param Y_0	����ԭ�㳣��		ʹ�� m ��λ
*	@param result �洢����Ľ���� B , L ��ʹ�� m ��λ
*/
void reverseGaussCoodTrans(double X, double Y, double L_0, double X_0, double Y_0, double* result);
