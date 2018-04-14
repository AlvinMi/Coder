#include "spi.h"

/******************************************************************************
* 1.����������ŵĸ��ù���, ʹ�� SPI2 CLOCK
* 2.��ʼ�� SPI2, ���� SPI2 ����ģʽ
* 3.ʹ�� SPI2
* 4.SPI ���ݴ���
* 5.�鿴 SPI ����״̬
******************************************************************************/

/******************************************************************************
* ����ԭ��: void SPI2_Config_Init(void)
* ��    ��: SPI2 ��ʼ��
* ��    ��: null
* �� �� ֵ: null
*******************************************************************************/ 
void SPI2_Config_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure; 
	GPIO_InitTypeDef  GPIO_InitStructure; 
		
	// �򿪶�Ӧ��ʱ��Դ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 
	
	/* Config SPI_NRF_SPI: 'SCK','MISO','MOSI' Pin */ 
	GPIO_InitStructure.GPIO_Pin = SPI_Pin_SCK| SPI_Pin_MISO| SPI_Pin_MOSI; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���ù���
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);
	/* SPI_NRF_SPI: CSN Pin */
	GPIO_InitStructure.GPIO_Pin = SPI_Pin_CSN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);	

	GPIO_SetBits(SPI_PORT, SPI_Pin_CSN);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 	// ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 	// ���ݴ�С 8 λ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 		// ʱ�Ӽ���, ����ʱΪ�� (0)
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ��һ��������Ч, ������Ϊ����ʱ�� 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 	// NSS �ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  // 4 ��Ƶ, 9MHz 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 	// ��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI2, &SPI_InitStructure); 

	/* Enable SPI2 */ 
	SPI_Cmd(SPI2, ENABLE);
}

/******************************************************************************
* ����ԭ��: u8 SPI_RW(u8 data);
* ��    ��: SPI ��д����
* ��    ��: д��� һ���ֽ�����
* �� �� ֵ: SPI ���յ���һ���ֽ�����
*******************************************************************************/ 
u8 SPI_RW(u8 data)
{
	/* �� SPI ���ͻ������ǿյȴ�| */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 	// �ο� st �ٷ��ĵ�
	/* ͨ�� SPI2 ����һ�ֽ����� */
	SPI_I2S_SendData(SPI2, data);
	/* �� SPI ���ջ�����Ϊ��ʱ�ȴ� */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	// �ο� st �ٷ��ĵ�
	/* Return the byte read from the SPI bus */ 
	return SPI_I2S_ReceiveData(SPI2); 
}

void SPI_CSN_H(void)
{
	GPIO_SetBits(SPI_PORT, SPI_Pin_CSN);
}

void SPI_CSN_L(void)
{
	GPIO_ResetBits(SPI_PORT, SPI_Pin_CSN);
}

//=============================================================================//
// дһ���ֽ�
u8 SPI_Write_Byte(u8 reg, u8 Value)
{
	u8 state;
	SPI_CSN_L();	// �������ֱ�Ӻ궨��(����IIC��) -->> eg: #define	CSN_L GPIO_XXX ->BSRR = SPI_Pin_CSN
	state = SPI_RW(reg);
	SPI_RW(Value);
	SPI_CSN_H();
	return state;
}

// ��һ���ֽ�
u8 SPI_Read_Byte(u8 reg)
{
	u8 value;
	SPI_CSN_L();
	SPI_RW(reg);			// ���ص���һ����Ч����
	value = SPI_RW(0);		// д����Ч����
	SPI_CSN_H();
	return value;
}

// д����ֽ�
u8 SPI_Write_nByte(u8 reg, u8 *pbuf, u8 len)
{
	u8 state;
	SPI_CSN_L();
	state = SPI_RW(reg);
	while(len)
	{
		SPI_RW(*pbuf);			// дһ������, ���׵�ַ����ȥ
		pbuf++;
		len--;
	}
	SPI_CSN_H();
	return state;
}

// ������ֽ�
u8 SPI_Read_nByte(u8 reg, u8 *pbuf, u8 len)
{
	u8 state;
	SPI_CSN_L();
	state = SPI_RW(reg);
	while(len)
	{
		*pbuf = SPI_RW(0);		// ͬ��д����Ч����
		pbuf ++;
		len--;
	}
	SPI_CSN_H();
	return state;
}



