#ifndef	__ANO_PID_H__
#define __ANO_PID_H__

#include "Ano_FcDate.h"

typedef struct
{
	u8 fb_d_mode;
	
	float kp;			// ����ϵ��
	float ki;			// ����ϵ��
	float kd_ex;		// ΢��ϵ��
	float kd_fb;		// previous_d ΢������
//	float inc_hz;		// ����ȫ΢�ֵ�ͨϵ��
//	float k_inc_d_norm;	// Incomplete  ����ȫ΢�� ��һ (0,1), �� 0~1
	float k_ff;			// ǰ��
}_PID_arg_st;

typedef struct
{
	float err;
	float exp_old;
	float feedback_old;
	
	float fb_d;
	float fb_d_ex;
	float exp_d;
//	float err_d_lpf;
	float err_i;
	float ff;
	float pre_d;
	
	float out;
	
}_PID_val_st;

// PID_���� ����
float PID_calculate(float T,		// ����
					float in_ff,	// ǰ��ֵ
					float expect,	// ����ֵ(�趨ֵ)
					float feedback,	// ����ֵ
					_PID_arg_st *pid_arg,	// PID �����ṹ��
					_PID_arg_st *pid_val,	// PID ���ݽṹ��
					float inte_d_lim,		// ��������޷�
					float inre_lim			// integration limit, �����޷�
					);

#endif


