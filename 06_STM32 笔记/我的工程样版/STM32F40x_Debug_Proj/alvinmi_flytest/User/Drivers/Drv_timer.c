#include "Drv_timer.h"
#include "Drv_led.h"

void TIM2_CONF(void)		// AHB1 84M
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	// ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// TIM_DeInit
	TIM_DeInit(TIMER2);
	
	// �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)
	TIM_TimeBaseStructure.TIM_Period = 1000;
	
	// �ۼ� TIM_Period ��Ƶ�ʺ����һ�����»����ж�, ʱ�ӷ�Ƶϵ��Ϊ 72
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
	
	// ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ, ����û���õ�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	// ����ģʽ -> ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��2
	TIM_TimeBaseInit(TIMER2, &TIM_TimeBaseStructure);
	
	// ���ж�, ����һ�����жϺ����������ж�.  
 	TIM_ClearFlag(TIMER2, TIM_FLAG_Update);
	
	// ����ʱ�������ж�(�ж�Դ)�� Update -> �����ж�;  TIM_FLAG_Trigger -> �����ж�
	TIM_ITConfig(TIMER2, TIM_IT_Update, ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIMER2, ENABLE);

	// �����εȴ�ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

void TIM2_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init ( &NVIC_InitStructure );
}

void Timer2_Init(void)
{
	TIM2_CONF();
	TIM2_NVIC();
	// ���¿�ʱ��, ��ʼ��ʱ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
}

//-----------------------------------------------------------------------------------//

void SysTick_Configuration(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	uint32_t	cnts;
	
	RCC_GetClocksFreq(&rcc_clocks);	// ��оƬ�ڸ���ģ���ʱ�ӱ����ڲ��� rcc_clocks ��, �����˽ṹ���е��ĸ�����
	
	cnts = (uint32_t) rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;		// HCLK_Frequency/1000 = 1000 
	cnts = cnts / 8;	// cnts/8 = 1000 
	
	SysTick_Config(cnts);
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	// SysTick ʱ��ԴΪ AHB / 8 
}

volatile uint32_t sysTickUpTime = 0;

/***************************************************************************
* ԭ�� Prototype: uint32_t GetSysTimer_us(void)
* ���� Function: get systime
* ���� Parameter: none
* ����ֵ Returned value: value
****************************************************************************/
uint32_t GetSysTime_us(void)
{
	register	uint32_t	ms;
	u32	value;
	ms = sysTickUpTime;
	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}

/***************************************************************************
* ԭ�� Prototype: void Delay_us (uint32_t us)
* ���� Function: Delay_us
* ���� Parameter: us
* ����ֵ Returned value: none
****************************************************************************/
void Delay_us (uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while ( GetSysTime_us() - now < us );
}

/***************************************************************************
* ԭ�� Prototype: void Delay_us (uint32_t us)
* ���� Function: Delay_us
* ���� Parameter: ms
* ����ֵ Returned value: none
****************************************************************************/
void Delay_ms(uint32_t ms)
{
	while ( ms-- )
        Delay_us ( 1000 );
}


u32 systime_ms;
void sys_time(void)
{
	systime_ms++;
}

u32 SysTick_GetTick(void)
{
	return systime_ms;
}

u32 Get_Delta_T(_get_dT_st *data)
{
    data->old = data->now;		 // ��һ�ε�ʱ��
    data->now = GetSysTime_us(); // ���ε�ʱ��
    data->dT = ( ( data->now - data->old ) );// ʱ��ļ��(����)
	
	if(data->init_flag == 0)
	{
		data->init_flag = 1; // ��һ�ε���ʱ��� 0 ��Ϊ��ʼ��, �Ժ���������� 
		return 0;
	}
	else
	{
		return data->dT;
	}
}

void SysTick_Handler(void)
{
	sysTickUpTime++;
	sys_time();
	LED_1ms_DRV();	// LED 1ms ����
}

//------------------------------------- end of file --------------------------------//
