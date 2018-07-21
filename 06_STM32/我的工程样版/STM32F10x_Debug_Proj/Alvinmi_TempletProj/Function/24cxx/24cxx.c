#include "24cxx.h" 
#include "delay.h"

/* 24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)*/

/******************************************************************************
* ����ԭ��:	void AT24CXX_Init(void)
* ��    ��: ��ʼ�� AT24cxx
* ��    ��:	null
* �� �� ֵ: null
*******************************************************************************/ 
void AT24CXX_Init(void)
{
	I2c_Soft_Init();
}

/******************************************************************************
* ����ԭ��:	u8 AT24CXX_Read1Byte(u16 readaddr)
* ��    ��: ��AT24CXXָ����ַ����һ������
* ��    ��:	readaddr ��ʼ�����ĵ�ַ
* �� �� ֵ: temp ����������
*******************************************************************************/ 
u8 AT24CXX_Read1Byte(u16 readaddr)
{				  
	u8 temp=0;		  	    																 
    I2c_Soft_Start();  
	if(EE_TYPE>AT24C16)
	{
		I2c_Soft_SendByte(0XA0);	   //����д����
		I2c_Soft_WaitAck();
		I2c_Soft_SendByte(readaddr>>8);//���͸ߵ�ַ
		I2c_Soft_WaitAck();		 
	}else I2c_Soft_SendByte(0XA0+((readaddr/256)<<1));   //����������ַ0XA0,д���� 	 

	I2c_Soft_WaitAck(); 
    I2c_Soft_SendByte(readaddr%256);   //���͵͵�ַ
	I2c_Soft_WaitAck();	    
	I2c_Soft_Start();  	 	   
	I2c_Soft_SendByte(0XA1);           //�������ģʽ			   
	I2c_Soft_WaitAck();	 
    temp=I2c_Soft_ReadByte(0);		   
    I2c_Soft_Stop();//����һ��ֹͣ����	    
	return temp;
}

/******************************************************************************
* ����ԭ��:	void AT24CXX_Write1Byte(u16 writeaddr,u8 data)
* ��    ��: ��AT24CXXָ����ַд��һ������
* ��    ��:	u16 writeaddr -> д�����ݵ�Ŀ�ĵ�ַ,u8 data -> Ҫд�������
* �� �� ֵ: null
*******************************************************************************/ 
void AT24CXX_Write1Byte(u16 writeaddr,u8 data)
{				   	  	    																 
    I2c_Soft_Start();  
	if(EE_TYPE>AT24C16)
	{
		I2c_Soft_SendByte(0XA0);	    //����д����
		I2c_Soft_WaitAck();
		I2c_Soft_SendByte(writeaddr>>8);//���͸ߵ�ַ
 	}else
	{
		I2c_Soft_SendByte(0XA0+((writeaddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	I2c_Soft_WaitAck();	   
    I2c_Soft_SendByte(writeaddr%256);   //���͵͵�ַ
	I2c_Soft_WaitAck(); 	 										  		   
	I2c_Soft_SendByte(data);     //�����ֽ�							   
	I2c_Soft_WaitAck();  		    	   
    I2c_Soft_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

/******************************************************************************
* ����ԭ��:	void AT24CXX_WriteLenByte(u16 writeaddr,u32 data,u8 len)
* ��    ��: ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
*			����д��16bit����32bit������.
* ��    ��:	u16 writeaddr,	��ʼд��ĵ�ַ  
*			u32 data,		���������׵�ַ
*			u8 len			Ҫд�����ݵĳ���2,4
* �� �� ֵ: null
*******************************************************************************/ 
void AT24CXX_WriteLenByte(u16 writeaddr,u32 data,u8 len)
{  	
	u8 t;
	for(t=0;t<len;t++)
	{
		AT24CXX_Write1Byte(writeaddr+t,(data>>(8*t))&0xff);
	}												    
}

/******************************************************************************
* ����ԭ��:	u32 AT24CXX_ReadLenByte(u16 readaddr,u8 len)
* ��    ��: ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
*			����д��16bit����32bit������.
* ��    ��:	u16 readaddr,	��ʼ�����ĵ�ַ 
*			u8 len			Ҫ�������ݵĳ���2,4
* �� �� ֵ: temp	��ȡ������
*******************************************************************************/ 
u32 AT24CXX_ReadLenByte(u16 readaddr,u8 len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_Read1Byte(readaddr+len-t-1); 	 				   
	}
	return temp;												    
}

/******************************************************************************
* ����ԭ��: u8 AT24CXX_Check(void)
* ��    ��: ���AT24CXX�Ƿ�����
*			��������24XX�����һ����ַ(255)���洢��־��.
*			���������24Cϵ��,�����ַҪ�޸�
* ��    ��:	null
* �� �� ֵ: 1: ʧ��; 0: ok
*******************************************************************************/ 
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_Read1Byte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_Write1Byte(255,0X55);
	    temp=AT24CXX_Read1Byte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

/******************************************************************************
* ����ԭ��: void AT24CXX_Read(u16 readaddr,u8 *pbuffer,u16 num)
* ��    ��: ��AT24CXX�����ָ����ַ��ʼ����ָ������������
* ��    ��:	u16 readaddr,	��ʼ�����ĵ�ַ ��24c02Ϊ0~255
*			u8 *pbuffer,	���������׵�ַ
*			u16 num			Ҫ�������ݵĸ���
* �� �� ֵ: null
*******************************************************************************/ 
void AT24CXX_Read(u16 readaddr,u8 *pbuffer,u16 num)
{
	while(num)
	{
		*pbuffer++=AT24CXX_Read1Byte(readaddr++);	
		num--;
	}
}  

/******************************************************************************
* ����ԭ��: void AT24CXX_Write(u16 writeaddr,u8 *pbuffer,u16 num)
* ��    ��: ��AT24CXX�����ָ����ַ��ʼд��ָ������������
* ��    ��:	u16 writeaddr,	��ʼд��ĵ�ַ ��24c02Ϊ0~255
*			u8 *pbuffer,	���������׵�ַ
*			u16 num			Ҫд�����ݵĸ���
* �� �� ֵ: null
*******************************************************************************/ 
void AT24CXX_Write(u16 writeaddr,u8 *pbuffer,u16 num)
{
	while(num--)
	{
		AT24CXX_Write1Byte(writeaddr,*pbuffer);
		writeaddr++;
		pbuffer++;
	}
}
 


