#include "stm32f10x.h"
#include "timer.h"

/**************************************************************
* TIMx(TIM6 & TIM7)  Ϊ������ʱ�� 
* TIMx(TIM1 & TIM8)  Ϊ�߼����ƶ�ʱ�� 
* TIMx(TIM2,TIM3,TIM4,TIM5) Ϊͨ�ö�ʱ�� 
***************************************************************/
// STM32 �Ķ�ʱ������ Timer6 & Timer7 ���������������� PWM ���
// ͬ���� STM32 ���� Timer6 & Timer7 �������������������벶��

// Timer3
uint32_t Timer3_Count = 0;	// ��¼ Timer3 �жϴ���
uint16_t Timer3_Frequency;	// Timer3 �ж�Ƶ��

// Timer4
uint32_t Timer4_Count = 0;	// ��¼ Timer3 �жϴ���
uint16_t Timer4_Frequency;	// Timer3 �ж�Ƶ��

//============================== Timer Fuction ================================//
/******************************************************************************
* ����ԭ��:	void Timer3_Init(uint16_t Handler_Frequency)
* ��    ��: Init Timer3
* ��    ��:	Handler_Frequency is Timer3 �ж�Ƶ��(IT_Frequency)
*******************************************************************************/ 
void Timer3_Init(uint16_t Handler_Frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	Timer3_Frequency = Handler_Frequency;	// Timer3 Ƶ��
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 1000*1000/Handler_Frequency ;	// װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;		// ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ���ָ�ʱ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// ���ϼ���
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);	// ����жϱ�־
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);	// ʹ�� Timer3
}

/******************************************************************************
* ����ԭ��:	void Timer3_pwm_init(u16 autoload, u16 prescale)
* ��    ��: Timer3 Pwm Init
* ��    ��:	autoload -->> �Զ�װ��ֵ & prescale -->> Ԥ��Ƶ����
* ��	��: Example: Timer3_pwm_init(899,0) -> ����Ƶ,PWMƵ��=72000000/900=80Khz
			�� Timer3_channel2 ��ӳ�䵽 PB5 ���� PWM
*******************************************************************************/ 
void Timer3_pwm_init(u16 autoload, u16 prescale)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;		 // Timer ����Ƚ�����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	// ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  // ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); // Timer3������ӳ��  TIM3_CH2->PB5    
 
   // ���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5; 		// TIM_CH2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;  // �����������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
   // ��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period		= autoload; // ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler		= prescale; // ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	// ��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2; // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High; // �������:TIM����Ƚϼ��Ը�
	
	// TIM ͨ������
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
	TIM_Cmd(TIM3, ENABLE);  // ʹ��TIM3
	
	/* �޸� TIM3_CCR2 ������ռ�ձ� */
	//TIM_SetCompare2(TIM3,0);	 	// ռ�ձȸ���ʵ��������޸�
}


/******************************************************************************
* ����ԭ��:	void Timer4_Init(uint16_t Handler_Frequency)
* ��    ��: Init Timer4
* ��    ��:	Handler_Frequency is Timer4 �ж�Ƶ��(IT_Frequency)
*******************************************************************************/ 
void Timer4_Init(uint16_t Handler_Frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	Timer4_Frequency = Handler_Frequency;	// Timer4 Ƶ��
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 1000*1000/Handler_Frequency ;	// װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;		// ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ���ָ�ʱ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// ���ϼ���
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);	// ����жϱ�־
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);	// ʹ�� Timer4
}

/******************************************************************************
* ����ԭ��:	void Timer5_InputCapture_Init(u16 autoload, u16 prescale)
* ��    ��: Timer5_Channel1 (PA0)  ���벶��, ���� PA0 �ϸߵ�ƽ����
* ��    ��:	autoload -> �Զ�װ��ֵ & prescale -> Ԥ��Ƶ����
			Example: Timer5_InputCapture_Init(0xFFFF,72-1) // �� 1MHz ��Ƶ�ʼ���
					 �� 1us ����һ��,  ����װ��ֵΪ���ֵ. 
*******************************************************************************/ 
void Timer5_InputCapture_Init(u16 autoload, u16 prescale)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
   	NVIC_InitTypeDef	NVIC_InitStructure;
	TIM_ICInitTypeDef	TIM5_ICInitStructure;	// Timer5_InputCapture_InitStructure

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	// ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  	// PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   // PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
	// PA0 ����
	// ��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period 		= autoload; 	// �趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler 	= prescale; 	// Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	// ��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	// �����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 // ���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;	// IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	// �жϷ����ʼ��	/* ���Կ��Ǽ��е� NVIC_Init() */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);// ��������ж� ,����CC1IE�����ж�	
   	TIM_Cmd(TIM5,ENABLE ); 	// ʹ�ܶ�ʱ��5
}

// ���������ȫ�ֱ���ֻ��Ϊ�˽��в������ʵ��.
u8  TIM5CH1_CAPTURE_STA=0;	// ���벶��״̬	 	    				
u16	TIM5CH1_CAPTURE_VAL;	// ���벶��ֵ
/******************************************************************************
* ����ԭ��:	void TIM5_IRQHandler(void)
* ��    ��: TIM5_IRQHandler �����ж�
* ��    ��:	None
*******************************************************************************/ 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}

