#include "BSP_Init.h"

/*--->>> ������� "Undefined symbol assert_failed" �������,����Ҫ�� stm32f4xx.h ��,
74 �����Ұ� #define USE_STDPERIPH_DRIVER ��������, ����������һ��Ҫ�������������.*/
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
		// ϵͳ����, �ͻ���뵽����
  }
}
#endif

int main(void)
{
	All_Init();
}






