#include "Ano_Pid.h"


float PID_calculate(float T,		// ����
					float in_ff,	// ǰ��ֵ
					float expect,	// ����ֵ(�趨ֵ)
					float feedback,	// ����ֵ
					_PID_arg_st *pid_arg,	// PID �����ṹ��
					_PID_arg_st *pid_val,	// PID ���ݽṹ��
					float inte_d_lim,		// ��������޷�
					float inre_lim			// integration limit, �����޷�
					)
{
	float differential, hz;
//	hz = safe_div(1.0f, dT_s, 0);
	
//	pid
}


