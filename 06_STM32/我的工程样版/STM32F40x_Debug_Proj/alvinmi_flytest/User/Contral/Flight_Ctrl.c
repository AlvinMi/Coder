#include "Flight_Ctrl.h"
#include "Attitude_Contrl.h"

/* PID ������ʼ�� */ 
void All_PID_Init(void)
{
	/*��̬���ƣ����ٶ� PID ��ʼ��*/
	Att_1level_PID_Init();
	
	/*��̬���ƣ��Ƕ� PID ��ʼ��*/
	Att_2level_PID_Init();
	
	/*�߶ȿ��ƣ��߶��ٶ� PID ��ʼ��*/

	
	/*�߶ȿ��ƣ��߶� PID ��ʼ��*/

	
	/*λ�ÿ��ƣ�λ���ٶ� PID ��ʼ��*/

}



