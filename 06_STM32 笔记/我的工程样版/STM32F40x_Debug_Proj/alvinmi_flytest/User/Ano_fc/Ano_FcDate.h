#ifndef __ANO_FCDATA_H__
#define __ANO_FCDATA_H__
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0 
enum pwminmode_e
{
	PWM = 0,
	PPM ,
	SBUS,
};


enum
{
	X = 0,
	Y = 1,
	Z = 2,
	VEC_XYZ,
};

enum
{
	ROL = 0,	// roll  �����
	PIT = 1,	// pitch ������
	YAW = 2,	// yaw 	 ƫ����
	VEC_RPY,	// ������ ? 
				// RPY -> roll��pitch��yaw		// rpy ��Ҳ�� X-Y-Z fixed angles.
				// ��һ����̬������ʽ, ��������������ת�� ��Ϊ Z-Y-X ŷ����. 
				// ����������ʽ��ͬ, �������ս������һ���ġ���Ԫ��תŷ������ 12 ����ת����.
};

enum
{
	KP = 0,
	KI = 1,
	KD = 2,
	PID,
};

typedef struct
{
	u8 first_f;
	float acc_offset[VEC_XYZ];
	float gyro_offset[VEC_XYZ];
	
	float surface_vec[VEC_XYZ];
	
	float mag_offset[VEC_XYZ];
	float mag_gain[VEC_XYZ];

} _save_st ;
extern _save_st save;









void Para_Data_Init(void);

#endif


