#include "Task_scheduler.h"
#include "Drv_timer.h"

u32 test_dT_1000hz[3],test_rT[6];	// Ӧ���ǲ����̺߳������������ִ��ʱ��



void Loop_1000Hz(void)	// 1ms ִ��һ��
{
	test_dT_1000hz[0] = test_dT_1000hz[1];
	test_rT[3] = test_dT_1000hz[1] = GetSysTime_us ();
	test_dT_1000hz[2] = (u32)(test_dT_1000hz[1] - test_dT_1000hz[0]);
//-------------------------------------------------------------------------------------------------------//
	/* ���������ݶ�ȡ */
//	Fc_Sensor_Get();
	
	/* ���Դ���������׼�� */
//	Sensor_Data_Prepare(1);
	
	/* ��̬������� */
//	IMU_Update_Task(1);
	
	/* ��ȡ WC_Z ���ٶ� */
//	WCZ_Acc_Get_Task();
	
	/* ����״̬���� */
//	Flight_State_Task(1,CH_N);
	
	/* ��̬���ٶȻ����� */
//	Att_1level_Ctrl(1e-3f);
	
	/* ���������� */
//	Motor_Ctrl_Task(1);
	
	/* �������ݽ���(��������) */
//	ANO_DT_Date_Exchange();
//-----------------------------------------------------------------------------------------------------//
	test_rT[4]= GetSysTime_us ();
	test_rT[5] = (u32)(test_rT[4] - test_rT[3]);
}

void Loop_500Hz(void)	// 2ms ִ��һ��
{

}

void Loop_200Hz(void)	// 5ms ִ��һ��
{

}

void Loop_100Hz(void)	// 10ms ִ��һ��
{
	test_rT[0]= GetSysTime_us ();	
//-----------------------------------------------------------------------------------------------------//
	/* ң�����ݴ��� */
//	RC_duty_task(10);
	
	/* ����ģʽ�������� */
//	Flight_Mode_Set(10);
	
	/* ��ȡ��̬�� (ROLL PITCH YAW) */
//	calculate_RPY();
	
	/* ��̬�ǶȻ����� */
//	Att_2level_Ctrl(10e-3f,CH_N);
	
	/* λ���ٶȻ����� (����) */
//	Loc_1level_Ctrl(10,CH_N);
	
	/* �߶������ں����� */
//	WCZ_Fus_Task(10);

	/* �߶��ٶȻ����� */
//	Alt_1level_Ctrl(10e-3f);

	/* �߶Ȼ����� */
//	Alt_2level_Ctrl(10e-3f);
	
	/* �ƹ���� */
//	LED_Task(10);
	
//----------------------------------------------------------------------------------------------------//
	test_rT[4] = GetSysTime_us ();			// 
	test_rT[5] = (u32)(test_rT[4] - test_rT[3]) ;	// 
}


void Loop_50Hz(void)	// 20ms ִ��һ��
{
	/* �������ݴ������� */
//	Mag_Update_Task(20);
}

void Loop_20Hz(void)	// 50ms ִ��һ��
{
	/* TOF �������� */
//	Drv_Vl53_RunTask();
	/* ��ѹ������� */
//	Power_UpdateTask(50);
}

void Loop_2Hz(void)		// 500ms ִ��һ��
{
	/* ��ʱ�������� */
//	Ano_Parame_Write_task(500);
}

// ϵͳ��������, ������ͬ��ִ��Ƶ�ʵ��߳� (�ṹ������)
static sched_task_t  sched_tasks[] = 
{
	{Loop_1000Hz,1000,  0, 0},		// 1ms
	{Loop_500Hz , 500,  0, 0},		// 2ms
	{Loop_200Hz , 200,  0, 0},		// 5ms
	{Loop_100Hz , 100,  0, 0},		// 10ms
	{Loop_50Hz  ,  50,  0, 0},		// 20ms
	{Loop_20Hz  ,  20,  0, 0},		// 50ms
	{Loop_2Hz   ,   2,  0, 0},		// 500ms
};

// �������鳤��, �ж��߳�����
#define	TASK_NUM	(sizeof(sched_tasks)/sizeof(sched_task_t))

void Scheduler_Setup(void)
{
	uint8_t	index = 0;
	// ��ʼ�������
	for(index=0; index < TASK_NUM; index++)
	{
		// ����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
		
		// �������Ϊ 1, Ҳ���� 1ms	(���ܴ��ڶ�ʱʱ��)
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}

// ��������ŵ� main() ��������ѭ�� while(1) ��, ��ͣ���ж��Ƿ����߳�Ӧ��ִ��.
void Scheduler_Run(void)
{
	uint8_t		index = 0;
	
	// ѭ���ж������߳�, �Ƿ�Ӧ��ִ��
	for(index=0; index < TASK_NUM; index++)
	{
		// ��ȡϵͳ��ǰ��ʱ��, ��λ ms
		uint32_t tnow = SysTick_GetTick();	
		
		// �����ж�, �����ǰʱ���ȥ��һ��ִ�е�ʱ��, ���ڵ��ڸ��̵߳�ִ������, ��ִ���߳�
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{	
			// �����̵߳�ʱ��, ������һ���ж�
			sched_tasks[index].last_run = tnow;
			
			// ִ���̺߳���, ʹ�ú���ָ��ָ����Ӧ���̺߳���
			sched_tasks[index].task_func();
		}
	}	
}


