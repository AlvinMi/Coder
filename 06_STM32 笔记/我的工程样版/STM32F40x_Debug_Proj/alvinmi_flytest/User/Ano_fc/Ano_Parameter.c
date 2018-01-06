#include "include.h"
#include "Ano_Parameter.h"
#include "Drv_w25qxx.h"

union Parameter Ano_Parame;


void Ano_Parame_Read(void)
{
	// ��ȡ��һ�����ڵĲ���
	Flash_SectorsRead(0x000000,&Ano_Parame.byte[0], 1);
	
	// �������û�б���ʼ��, ����в�����ʼ������.
	if(Ano_Parame.set.frist_init != SOFT_VER)
	{
		Parame_Reset();
//		PID_Rest();
//		Ano_Parame_Write();
	}
	// 
//	Parame_Copy_Para2fc();
}

static void Parame_Copy_Para2fc(void)
{
	for(u8 i=0; i < 3; i++)
	{
//		save.acc_offset[i]		=	Ano_Parame.set.acc_offset[i];
//		save.gyro_offset[i]		=	Ano_Parame.set.gyro_offset[i];
//		save.mag_offset[i]		=	Ano_Parame.set.mag_offset[i];  
//		save.mag_gain[i]		=	Ano_Parame.set.mag_gain[i];  
		
	}
}
	
void Parame_Reset(void)
{
	// ������ʼ��
//	Ano_Parame.set.pwmInMode =  PWM;		// 	PWM Ϊö��
	Ano_Parame.set.warn_power_voltage = 3.55 * 3;
	Ano_Parame.set.return_home_power_voltage = 3.7f * 3;
	Ano_Parame.set.lowest_power_voltage = 3.5f * 3;
	
	Ano_Parame.set.auto_take_off_height = 0;    // cm   
	
	for(u8 i = 0;i<3;i++)	// Ϊʲô�� 8��?
	{
//		Ano_Parame.set.acc_offset[i] = 0;		// ���ٶȼ���ƫ
//		Ano_Parame.set.gyro_offset[i] = 0;		// ��������ƫ
//		Ano_Parame.set.mag_offset[i] = 0;  		// ��������ƫ
//		Ano_Parame.set.mag_gain[i] = 1; 		// ������У������
//		
//		Ano_Parame.set.center_pos_cm[i] = 0;	// ������Դ�����λ��ƫ����
	}
	
	// ������ copy ���ɿ�
	Parame_Copy_Para2fc();
	
//	ANO_DT_SendString("parameter reset!",sizeof("parameter reset!"));
}


