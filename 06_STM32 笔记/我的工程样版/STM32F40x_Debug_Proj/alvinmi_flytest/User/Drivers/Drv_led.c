#include "Drv_led.h"



/***************************************************************************
* ԭ�� Prototype: void LED_1ms_DRV(void)
* ���� Function:  1 ms LED ����, �� 1ms ��ʱ���ж������
* ���� Parameter: none
* ����ֵ Returned value: none
****************************************************************************/
float LED_Brightness[4] = {0,20,0,0}; 	//TO 20 //XBRG

void LED_1ms_DRV(void)	//0~20
{
	static u16 led_cnt[4];
	
	u8 i;
	
	for(i=0;i<4;i++)
	{
		if(led_cnt[i] < LED_Brightness[i])
		{
			switch(i)
			{
				
			}
		}
		else
		{
		
		}
	}
	
}






