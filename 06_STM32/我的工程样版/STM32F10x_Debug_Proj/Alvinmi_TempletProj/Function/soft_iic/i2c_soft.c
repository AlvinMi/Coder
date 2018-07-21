#include "i2c_soft.h"

void I2C_Delay(void)
{
   /*u8 i=0; 
   while(i) 
   { 
     i--; 
   } 
		*/ 	
}

/******************************************************************************
* ����ԭ��:	void I2c_Soft_Init(void)	ģ�� IIC ��ʼ��
* ��    ��: IIC_Soft Init
* ��    ��:	null
* �� �� ֵ: null
*******************************************************************************/ 
void I2c_Soft_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_I2C, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = I2C_Pin_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIO_I2C, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = I2C_Pin_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
}

/******************************************************************************
* ����ԭ��:	int I2c_Soft_Start(void)
* ��    ��: iic Start	IIC ��ʼ�ź�
* ��    ��:	null
* �� �� ֵ: int
*******************************************************************************/ 
int I2c_Soft_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_Delay();
	if(!SDA_Read) return 0;		// SDA ��Ϊ�͵�ƽ������æ, �˳�. -> SDA = 0, return.
	SDA_L;
	I2C_Delay();
	if(!SDA_Read) return 0;		// SDA ��Ϊ�ߵ�ƽ�����ߴ���, �˳�. -> SDA = 1, ERROR
	SDA_L;
	I2C_Delay();
	return 1;
}

/******************************************************************************
* ����ԭ��:	void I2c_Soft_Stop(void)
* ��    ��: iic Stop	IIC ֹͣ�ź�
* ��    ��:	null
* �� �� ֵ: null
*******************************************************************************/ 
void I2c_Soft_Stop(void)
{
	SCL_L;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
}

/******************************************************************************
* ����ԭ��:	void I2c_Soft_Ack(void)
* ��    ��: iic ACK		IIC Ӧ���ź�
* ��    ��:	null
* �� �� ֵ: null
*******************************************************************************/ 
void I2c_Soft_Ack(void)
{
	SCL_L;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
}

/******************************************************************************
* ����ԭ��:	void I2c_Soft_NoAck(void)
* ��    ��: iic NoACK		IIC ��Ӧ���ź�
* ��    ��:	null
* �� �� ֵ: null
*******************************************************************************/ 
void I2c_Soft_NoAck(void)
{
	SCL_L;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
}

/******************************************************************************
* ����ԭ��:	int I2c_Soft_WaitAck(void)
* ��    ��: iic WaitACK		return 1 -- ACK; return 0 NoAck.
* ��    ��:	null
* �� �� ֵ: int 
*******************************************************************************/ 
int I2c_Soft_WaitAck(void)
{
	u8 ErrorTime = 0;
	SCL_L;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	if(SDA_Read)
	{
		ErrorTime ++;
		if(ErrorTime > 200)
		{
			I2c_Soft_Stop();
			return 1;
		}
	}
	SCL_L;
	I2C_Delay();
	return 0;
}

/******************************************************************************
* ����ԭ��:	void I2c_Soft_SendByte(u8 SendByte)
* ��    ��: iic SendByte		���ݴӸ�λ����λ 
* ��    ��:	u8 SendByte
* �� �� ֵ: null
*******************************************************************************/ 
void I2c_Soft_SendByte(u8 SendByte)
{
	u8 i = 8;
	while(i--)
	{
		SCL_L;
		I2C_Delay();
		if(SendByte&0x80)
		{
			SDA_H;
		}else
		{
			SDA_L;
			SendByte <<=1;
			I2C_Delay();
			SCL_H;
			I2C_Delay();
		}
	}
	SCL_L;
}

/******************************************************************************
* ����ԭ��:	u8 I2c_Soft_ReadByte(u8 ask)
* ��    ��: iic ReadByte		���ݴӸ�λ����λ 
			�� 1 ���ֽ�, ack = 1ʱ, ���� Ack, ack = 0ʱ, ���� NoAck
* ��    ��:	u8 ask
* �� �� ֵ: ��������ֽ�
*******************************************************************************/ 
u8 I2c_Soft_ReadByte(u8 ask)
{
	u8 i = 8;
	u8 ReceiveByte = 0;
	
	SDA_H;
	while(i--)
	{
		ReceiveByte <<= 1;
		SCL_L;
		I2C_Delay();
		SCL_H;
		I2C_Delay();
		if(SDA_Read)
		{
			ReceiveByte |= 0x01; 
		}
	}
	SCL_L;
	
	if(ask)
		I2c_Soft_Ack();
	else
		I2c_Soft_NoAck();
	
	return ReceiveByte;
}

/******************************************************************************
* ����ԭ��:	u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
* ��    ��: IIC Write 1Byte Date.		IIC дһ���ֽ�����
* ��    ��:	u8 SlaveAddress
			u8 REG_Address
			u8 REG_data
* �� �� ֵ: 0 | 1
*******************************************************************************/ 
u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1);
	if(I2c_Soft_WaitAck())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address);		
	I2c_Soft_WaitAck();
	I2c_Soft_SendByte(REG_data);
	I2c_Soft_WaitAck();
	I2c_Soft_Stop();
	return 0;
}

/******************************************************************************
* ����ԭ��:	u8 IIC_Read_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
* ��    ��: IIC Read 1Byte Date.		IIC ��һ���ֽ�����
* ��    ��:	u8 SlaveAddress
			u8 REG_Address
			u8 REG_data
* �� �� ֵ: 0 | 1
*******************************************************************************/ 
u8 IIC_Read_1Byte(u8 SlaveAddress,u8 REG_Address,u8 *REG_data)
{
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAck())	
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address<<1);
	I2c_Soft_WaitAck();
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1 | 0x01);
	I2c_Soft_WaitAck();
	*REG_data = I2c_Soft_ReadByte(0);
	I2c_Soft_Stop();
	return 0;
}

/******************************************************************************
* ����ԭ��:	u8 IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
* ��    ��: IIC Write nByte Date.		IIC д n ���ֽ�����
* ��    ��:	u8 SlaveAddress
			u8 REG_Address
			u8 len
			u8 *buf
* �� �� ֵ: 0 | 1
*******************************************************************************/ 
u8 IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAck())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address); 
	I2c_Soft_WaitAck();
	while(len--) 
	{
		I2c_Soft_SendByte(*buf++); 
		I2c_Soft_WaitAck();
	}
	I2c_Soft_Stop();
	return 0;
}

/******************************************************************************
* ����ԭ��:	u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
* ��    ��: IIC Read nByte Date.		IIC �� n ���ֽ�����
* ��    ��:	u8 SlaveAddress
			u8 REG_Address
			u8 len
			u8 *buf
* �� �� ֵ: 0 | 1
*******************************************************************************/ 
u8 mpu_test;

u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{
	mpu_test = I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAck())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address); 
	I2c_Soft_WaitAck();
	
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1 | 0x01); 
	I2c_Soft_WaitAck();
	while(len) 
	{
		if(len == 1)
		{
			*buf = I2c_Soft_ReadByte(0);
		}
		else
		{
			*buf = I2c_Soft_ReadByte(1);
		}
		buf++;
		len--;
	}
	I2c_Soft_Stop();
	return 0;
}


