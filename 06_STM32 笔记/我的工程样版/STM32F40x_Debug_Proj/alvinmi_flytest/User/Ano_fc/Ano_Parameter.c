#include "stm32f4xx.h"
#include "include.h"
#include "Ano_Parameter.h"
#include "Drv_w25qxx.h"

#include "Flight_Ctrl.h"

union Parameter Ano_Parame;

// PID Rest
void PID_Rest(void)
{
//---	��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[ROL][KP]	= 3.5f;  	// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[ROL][KI]	= 0; 		// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[ROL][KD]	= 0.1f; 	// ��̬���ƽ��ٶȻ� PID ����
	
	Ano_Parame.set.pid_att_1level[PIT][KP]	= 3.5f;		// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[PIT][KI]	= 0;		// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[PIT][KD]	= 0.1f;		// ��̬���ƽ��ٶȻ� PID ����
	
	Ano_Parame.set.pid_att_1level[YAW][KP]	= 4.0f; 	// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[YAW][KI]	= 0.f; 		// ��̬���ƽ��ٶȻ� PID ����
	Ano_Parame.set.pid_att_1level[YAW][KD]	= 0.0f; 	// ��̬���ƽ��ٶȻ� PID ����
	
//---   ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[ROL][KP]	= 5.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[ROL][KI]	= 3.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[ROL][KD]	= 0.0f;		// ��̬���ƽǶȻ� PID ����
	
	Ano_Parame.set.pid_att_2level[PIT][KP]	= 5.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[PIT][KI]	= 3.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[PIT][KD]	= 0.0f;		// ��̬���ƽǶȻ� PID ����
	
	Ano_Parame.set.pid_att_2level[YAW][KP]	= 5.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[YAW][KI]	= 1.0f;		// ��̬���ƽǶȻ� PID ����
	Ano_Parame.set.pid_att_2level[YAW][KD]	= 0.8f;		// ��̬���ƽǶȻ� PID ����
		
//---	�߶ȿ��Ƹ߶��ٶȻ� PID ����
	Ano_Parame.set.pid_alt_1level[KP]		= 2.0f;		// �߶ȿ��Ƹ߶��ٶȻ� PID ����
	Ano_Parame.set.pid_alt_1level[KI]		= 2.0f;		// �߶ȿ��Ƹ߶��ٶȻ� PID ����
	Ano_Parame.set.pid_alt_1level[KD]		= 0.02f;	// �߶ȿ��Ƹ߶��ٶȻ� PID ����
	
//---	�߶ȿ��Ƹ߶Ȼ�	PID ����
	Ano_Parame.set.pid_alt_2level[KP]		= 1.0f;		// �߶ȿ��Ƹ߶Ȼ� PID ����
	Ano_Parame.set.pid_alt_2level[KI]		= 0;		// �߶ȿ��Ƹ߶Ȼ� PID ����
	Ano_Parame.set.pid_alt_2level[KD]		= 0;		// �߶ȿ��Ƹ߶Ȼ� PID ����
	
//---	λ�ÿ���λ���ٶȻ� PID ����						
	Ano_Parame.set.pid_loc_1level[KP]		= 0;		// λ�ÿ���λ���ٶȻ� PID ����
	Ano_Parame.set.pid_loc_1level[KI]		= 0;		// λ�ÿ���λ���ٶȻ� PID ����
	Ano_Parame.set.pid_loc_1level[KD]		= 0;		// λ�ÿ���λ���ٶȻ� PID ����
	
//---	λ�ÿ���λ�û� PID ����
	Ano_Parame.set.pid_loc_2level[KP]		= 0;		// λ�ÿ���λ�û� PID ����
	Ano_Parame.set.pid_loc_2level[KI]		= 0;		// λ�ÿ���λ�û� PID ����
	Ano_Parame.set.pid_loc_2level[KD]		= 0;		// λ�ÿ���λ�û� PID ����
	
//	ANO_DT_SendString("PID reset!",sizeof("PID reset!"));
}

static void Ano_Parame_Write(void)
{
	All_PID_Init();			// ���� PID ������, ���³�ʼ�� PID
	
	
}

void Ano_Parame_Read(void)
{
	// ��ȡ��һ�����ڵĲ���
	Flash_SectorsRead(0x000000,&Ano_Parame.byte[0], 1);		// ������ 4096KB
	
	// �������û�б���ʼ��, ����в�����ʼ������.
	if(Ano_Parame.set.frist_init != SOFT_VER)
	{
		Parame_Reset();				// ������ʼ��
		PID_Rest();					// PID_Rest
		Ano_Parame_Write();			// Write_Parame
	}
	// 
//	Parame_Copy_Para2fc();
}

static void Parame_Copy_Para2fc(void)
{
	for(u8 i=0; i<3; i++)
	{
		save.acc_offset [i]	 =	Ano_Parame.set.acc_offset[i];
		save.gyro_offset[i]	 =	Ano_Parame.set.gyro_offset[i];
		save.mag_offset [i]	 =	Ano_Parame.set.mag_offset[i];  
		save.mag_gain   [i]	 =	Ano_Parame.set.mag_gain[i];    

		// icm 20602
//		Center_Pos_Set();		// ����λ������?	
	}
}
	
void Parame_Reset(void)
{
	// ������ʼ�� Parameter.Parameter_s = Ano_Parame.set
	Ano_Parame.set.pwmInMode =  PWM;		// 	PWM Ϊö��	Parameter.Parameter_s.pwmInMode
	Ano_Parame.set.warn_power_voltage = 3.55 * 3;
	Ano_Parame.set.return_home_power_voltage = 3.7f * 3;
	Ano_Parame.set.lowest_power_voltage = 3.5f * 3;
	
	Ano_Parame.set.auto_take_off_height = 0;    // cm   
	
	for(u8 i = 0;i<3;i++)	// Ϊʲô��  3��
	{
		Ano_Parame.set.acc_offset [i] = 0;		// ���ٶȼ���ƫ
		Ano_Parame.set.gyro_offset[i] = 0;		// ��������ƫ
		Ano_Parame.set.mag_offset [i] = 0;  	// ��������ƫ
		Ano_Parame.set.mag_gain   [i] = 1; 		// ������У������
		
		Ano_Parame.set.center_pos_cm[i] = 0;	// ������Դ�����λ��ƫ����
	}
	
	// ������ copy ���ɿ�
	Parame_Copy_Para2fc();
	
//	ANO_DT_SendString("parameter reset!",sizeof("parameter reset!"));
}


