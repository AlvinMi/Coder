#ifndef __24CXX_H
#define __24CXX_H
#include "i2c_soft.h"   

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//This use is: 24c02�����Զ��� EE_TYPE Ϊ AT24C02
#define EE_TYPE AT24C02
					  
u8 AT24CXX_Read1Byte(u16 readaddr);							// ָ����ַ��ȡһ���ֽ�
void AT24CXX_Write1Byte(u16 writeaddr,u8 data);				// ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 writeaddr,u32 data,u8 len);	// ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 readaddr,u8 len);				// ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Read(u16 readaddr,u8 *pbuffer,u16 num);   		// ��ָ����ַ��ʼ����ָ�����ȵ�����
void AT24CXX_Write(u16 writeaddr,u8 *pbuffer,u16 num);		// ��ָ����ַ��ʼд��ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC
#endif
















