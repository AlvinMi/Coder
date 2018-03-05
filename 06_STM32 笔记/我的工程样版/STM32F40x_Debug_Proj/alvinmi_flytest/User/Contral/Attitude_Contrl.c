#include "Attitude_Contrl.h"
#include "Ano_FcDate.h"
#include "Ano_Parameter.h"
#include "Ano_Pid.h"

// ���ٶȿ��ƻ�����
_PID_arg_st  arg_1[VEC_RPY];

// �ǶȻ����Ʋ���
_PID_arg_st  arg_2[VEC_RPY];



/*
��̬�����ʲ��ֿ��Ʋ���

arg_1_kp:  �������ٶ���Ӧ�ٶ�, ���𵴵�ǰ����, ����Խ��Խ��.

��ʱ, ���Խ��� arg_1_kp, ���� arg_1_kd .

������ arg_1_kd �Ѿ�����������, ��Ҫ�� kp �� kd ͬʱ��С. 
*/
#define CTRL_1_KI_START 0.f


/*���ٶȻ�PID������ʼ��*/
void Att_1level_PID_Init(void)
{
	arg_1[ROL].kp = Ano_Parame.set.pid_att_1level[ROL][KP];
	arg_1[ROL].ki = Ano_Parame.set.pid_att_1level[ROL][KI];
	arg_1[ROL].kd_ex = 0.005f;
	arg_1[ROL].kd_fb = Ano_Parame.set.pid_att_1level[ROL][KD];
	arg_1[ROL].k_ff = 0.0f;
	
	arg_1[PIT].kp = Ano_Parame.set.pid_att_1level[PIT][KP];
	arg_1[PIT].ki = Ano_Parame.set.pid_att_1level[PIT][KI];
	arg_1[PIT].kd_ex = 0.005f;
	arg_1[PIT].kd_fb = Ano_Parame.set.pid_att_1level[PIT][KD];
	arg_1[PIT].k_ff  = 0.0f;

	arg_1[YAW].kp = Ano_Parame.set.pid_att_1level[YAW][KP];
	arg_1[YAW].ki = Ano_Parame.set.pid_att_1level[YAW][KI];
	arg_1[YAW].kd_ex = 0.005f;
	arg_1[YAW].kd_fb = Ano_Parame.set.pid_att_1level[YAW][KD];
	arg_1[YAW].k_ff  = 0.0f;

#if (MOTOR_ESC_TYPE == 2)	// ��ˢ�����ɲ���ĵ������.
	#define DIFF_GAIN 0.3f
//	arg_1[ROL].kd_ex = arg_1[ROL].kd_ex *DIFF_GAIN;
//	arg_1[PIT].kd_ex = arg_1[PIT].kd_ex *DIFF_GAIN;
	arg_1[ROL].kd_fb = arg_1[ROL].kd_fb *DIFF_GAIN;
	arg_1[PIT].kd_fb = arg_1[PIT].kd_fb *DIFF_GAIN;
	
#elif (MOTOR_ESC_TYPE == 1)	// ��ˢ�������ɲ���ĵ��.
	#define DIFF_GAIN 1.0f
//	arg_1[ROL].kd_ex = arg_1[ROL].kd_ex *DIFF_GAIN;
//	arg_1[PIT].kd_ex = arg_1[PIT].kd_ex *DIFF_GAIN;
	arg_1[ROL].kd_fb = arg_1[ROL].kd_fb *DIFF_GAIN;
	arg_1[PIT].kd_fb = arg_1[PIT].kd_fb *DIFF_GAIN;
	
#endif
	
}


void Att_2level_PID_Init(void)
{
	arg_2[ROL].kp = Ano_Parame.set.pid_att_2level[ROL][KP];
	
}


