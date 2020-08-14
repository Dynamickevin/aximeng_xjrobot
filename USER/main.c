
#include "includes.h"

/////////////////////////UCOSII��������///////////////////////////////////
//START ��ʼ����
//�����������ȼ�
//#define APP_TASK_START_PRIO      			0 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
//#define APP_TASK_START_STK_SIZE  				128
//��ʼ���� �����ջ	
static OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
//��ʼ������
void App_TaskStart(void *pdata);	
 	
//�����������ȼ�
//#define WEATHER_TASK_PRIO      			3
//���������ջ��С
//#define WEATHER_LENGH								100	
//��ӡ������� �����ջ	
OS_STK TaskStk_Weather[WEATHER_LENGH];
//��ӡ���������
void Task_Weather(void *pdata);

//�����������ȼ�
//#define REMOTE_REC_TASK_PRIO      			5
//���������ջ��С
//#define REMOTE_REC_LENGH								200	
//����������� �����ջ	
 OS_STK TaskStk_Remote_Rec[REMOTE_REC_LENGH];
//�������������
void Task_Remote_Rec(void *pdata);

//�����������ȼ�
//#define TIMER_TASK_PRIO      			7
//���������ջ��С
//#define TIMER_LENGH								100	
//ʱ�䶨ʱ�������ջ	
 OS_STK TaskStk_Timer[TIMER_LENGH];
//ʱ�䶨ʱ��������
void Task_Timer(void *pdata);

//�����������ȼ�
//#define LOCAL_TASK_PRIO      			9
//���������ջ��С
//#define LOCAL_LENGH								200	
//�������� �����ջ	
 OS_STK TaskStk_Local[LOCAL_LENGH];
//����������
void Task_Local(void *pdata);

//�����������ȼ�
//#define SENSOR_DATA_TASK_PRIO      			11
//���������ջ��С
//#define SENSOR_DATA_LENGH								100	
//�������ɼ��������� �����ջ	
 OS_STK TaskStk_Sensor_Data_Collect[SENSOR_DATA_LENGH];
//�������ɼ�������
void Task_Sensor_Data_Collect(void *pdata);	
 
////�����ջ�ռ����
//static  OS_STK     App_TaskStartStk[APP_TASK_START_STK_SIZE]; //��ʼ�����ջ
//static  OS_STK		 TaskStk_Weather [WEATHER_LENGH];			//��ӡ��������ջ
//static  OS_STK		 TaskStk_Remote_Rec [REMOTE_REC_LENGH];		//������������ջ
//static  OS_STK		 TaskStk_Timer [TIMER_LENGH];				//ʱ�䶨ʱ�������ջ
//static  OS_STK		 TaskStk_Local [LOCAL_LENGH];				//���������ջ
//static  OS_STK	 TaskStk_Sensor_Data_Collect[SENSOR_DATA_LENGH];//�������ɼ������ջ




int main(void)
{ 
 
	CPU_INT08U  os_err;
	os_err = os_err; 
   __disable_irq();	// �ر�ȫ���жϣ�ucosiiҪ������ȹر�ȫ���ж�
	SystemInit();
	delay_init(168);		  //��ʼ����ʱ����
	bsp_Led_Init();		    //��ʼ��LED�˿�
	
	OSInit();   
 	//OSTaskCreate(App_TaskStart,(void *)0,(OS_STK *)&App_TaskStartStk[APP_TASK_START_STK_SIZE-1],APP_TASK_START_PRIO );//������ʼ����
	//������ʼ����
	os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task. */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
														 
	OSStart();	
}

 //��ʼ����
void App_TaskStart(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	CPU_INT08U  os_err;
	os_err = os_err; 
	pdata = pdata;
	
  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)
	
 	//OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	//OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
	
	//��ӡ�������	 ���ȼ�3
    os_err = OSTaskCreateExt((void (*)(void *)) Task_Weather, 		//��������ָ��	
														(void 		 * ) 0, 						//������ʼִ��ʱ���ݸ�����Ĳ�����ָ��
														(OS_STK		 * )&TaskStk_Weather[WEATHER_LENGH - 1],	//���������Ķ�ջ��ջ��ָ��	
														(INT8U		   ) WEATHER_TASK_PRIO,						//�������������ȼ�
														(INT16U          ) WEATHER_TASK_ID,						//ΪҪ���������񴴽�һ������ı�ʶ��
														(OS_STK        * )&TaskStk_Weather[0],					//ָ������Ķ�ջջ�׵�ָ�룬���ڶ�ջ�ļ���
														(INT32U          ) WEATHER_LENGH,						//����ָ����ջ��Ա��Ŀ������,���ڶ�ջ�ļ���
														(void          * )0,									//ָ���û����ӵ��������ָ��
														(INT16U         )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));	//ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ���㣬�����Ƿ�Ҫ���и�������ȵ�						
		
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(WEATHER_TASK_PRIO, "Task_Weather", &os_err);
	#endif			
		OSTimeDly(OS_TICKS_PER_SEC);	
		
//�����������  ���ȼ�5
   os_err = OSTaskCreateExt((void (*)(void *)) Task_Remote_Rec, 		//��������ָ��	
													(void 		 * ) 0, 						//������ʼִ��ʱ���ݸ�����Ĳ�����ָ��
													(OS_STK		 * )&TaskStk_Remote_Rec[REMOTE_REC_LENGH - 1],	//���������Ķ�ջ��ջ��ָ��	
													(INT8U		   ) REMOTE_REC_TASK_PRIO,						//�������������ȼ�
													(INT16U        ) REMOTE_REC_TASK_ID,						//ΪҪ���������񴴽�һ������ı�ʶ��
													(OS_STK      * )&TaskStk_Remote_Rec[0],						//ָ������Ķ�ջջ�׵�ָ�룬���ڶ�ջ�ļ���
													(INT32U        ) REMOTE_REC_LENGH,							//����ָ����ջ��Ա��Ŀ������,���ڶ�ջ�ļ���
													(void        * )0,											//ָ���û����ӵ��������ָ��
													(INT16U        )(OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR)	);	//ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ���㣬�����Ƿ�Ҫ���и�������ȵ�						
		
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(REMOTE_REC_TASK_PRIO, "Task_Remote_Rec", &os_err);
	#endif	
				
	//ʱ�� ��ʱ������  ���ȼ�7
    os_err = OSTaskCreateExt (Task_Timer,				//��������ָ��
					(void *)0,            				//������ʼִ��ʱ���ݸ�����Ĳ�����ָ��
					&TaskStk_Timer[TIMER_LENGH - 1],	//���������Ķ�ջ��ջ��ָ��
					TIMER_TASK_PRIO,					//�������������ȼ�
					TIMER_TASK_ID,						//ΪҪ���������񴴽�һ������ı�ʶ��
					&TaskStk_Timer[0],					//ָ������Ķ�ջջ�׵�ָ�룬���ڶ�ջ�ļ���
					TIMER_LENGH,						//����ָ����ջ��Ա��Ŀ������,���ڶ�ջ�ļ���
					(void *)0,							//ָ���û����ӵ��������ָ��
					OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR//ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ���㣬�����Ƿ�Ҫ���и�������ȵ�
					);
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(TIMER_TASK_PRIO, "Task_Timer", &os_err);
	#endif						
	 
   
	 //��������				���ȼ�9																									
	 os_err =OSTaskCreateExt(Task_Local,					//��������ָ��
					(void *)0,            					//������ʼִ��ʱ���ݸ�����Ĳ�����ָ��
					&TaskStk_Local[LOCAL_LENGH - 1],		//���������Ķ�ջ��ջ��ָ��
					LOCAL_TASK_PRIO,						//�������������ȼ�
					LOCAL_TASK_ID,							//ΪҪ���������񴴽�һ������ı�ʶ��
					&TaskStk_Local[0],						//ָ������Ķ�ջջ�׵�ָ�룬���ڶ�ջ�ļ���
					LOCAL_LENGH,							//����ָ����ջ��Ա��Ŀ������,���ڶ�ջ�ļ���
					(void *)0,								//ָ���û����ӵ��������ָ��
					OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR		//ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ���㣬�����Ƿ�Ҫ���и�������ȵ�
					);
    #if OS_TASK_NAME_EN > 0
		OSTaskNameSet(LOCAL_TASK_PRIO, "Task_Local", &os_err);
	#endif
		
	//���������ݲɼ����� ���ȼ�11
	os_err =OSTaskCreateExt (Task_Sensor_Data_Collect,		
					(void *)0,            
					&TaskStk_Sensor_Data_Collect[SENSOR_DATA_LENGH - 1],
					SENSOR_DATA_TASK_PRIO,
					SENSOR_DATA_ID,
					&TaskStk_Sensor_Data_Collect[0],
					SENSOR_DATA_LENGH,
					(void *)0,
					OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR
					);	
	#if OS_TASK_NAME_EN > 0
			OSTaskNameSet(SENSOR_DATA_TASK_PRIO, "Task_Sensor_Data_Collect", &os_err);
	#endif		

	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
					
} 




