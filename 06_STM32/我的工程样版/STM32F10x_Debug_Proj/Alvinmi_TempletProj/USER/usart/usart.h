#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Uart1_Init(uint32_t baud);


#endif
