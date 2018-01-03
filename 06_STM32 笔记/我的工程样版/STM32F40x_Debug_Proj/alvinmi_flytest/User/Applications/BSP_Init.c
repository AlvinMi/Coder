#include "BSP_Init.h"
#include "Drv_timer.h"
#include "Drv_led.h"
#include "Drv_w25qxx.h"




u8 All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		// �ж����ȼ��������

	SysTick_Configuration();	// SysTick Init
	
	Drv_LED_Init();				// LED ���ܳ�ʼ��, �� Init LED GPIO.
	
	Flash_Init();               // ���� Flash оƬ������ʼ��.

	Para_Data_Init();           // �������ݳ�ʼ��

	
	return 0;
}





