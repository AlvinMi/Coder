#ifndef		__TASK_SCHEDULER__
#define 	__TASK_SCHEDULER__
#include "stm32f4xx.h"

typedef	struct
{
void (*task_func)(void);
uint16_t	rate_hz;			// Ƶ��
uint16_t	interval_ticks;		// �δ���(�δ�ʱ��Ϊ 1ms)
uint32_t	last_run;			// �ϴ�����(��¼�ϴ����е�ʱ��)
}sched_task_t;

void Scheduler_Setup(void);
void Scheduler_Run(void);


#endif

