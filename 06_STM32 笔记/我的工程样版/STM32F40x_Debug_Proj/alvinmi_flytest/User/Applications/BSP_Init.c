#include "BSP_Init.h"
#include "Drv_timer.h"


u8 All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		// �ж����ȼ��������

	SysTick_Configuration();	// SysTick Init
	
	return 0;
}





