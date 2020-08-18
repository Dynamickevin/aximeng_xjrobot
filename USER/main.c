
#include <includes.h>
#include "stm32f4xx.h"                  // Device header file

OS_EVENT	*log_sem;

OS_EVENT	*PtzSem;


DATA_CONTROL_BLOCK 		msgBuffer[100];
OS_MEM								*p_msgBuffer;					//OS_MEM���ڴ�������ݽṹ

OS_MEM								*mem160ptr;
INT8U		 							mem_160[MEM160_NUM][220];

OS_EVENT							*mem512_sem;
OS_MEM								*mem512ptr; 
INT8U									 mem_512[MEM512_NUM][512];

extern int				 			gui_key;
extern OS_EVENT  				*uart_q;
extern void 		    		*RcvUARTQ[Q_MAX_SIZE];
extern OS_EVENT    	    *sem_time;
extern RbtState         gRbtState;


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

//���г���
#define 	REMOTEREC_Q_LEN		32
#define 	LOCAL_Q_LEN				10
#define		WEATHER_Q_LEN			32
#define		TIMER_Q_LEN				32
#define		SENSOR_DATA_COLLECT_Q_LEN	32

void *local_Q[LOCAL_Q_LEN]; 			//���ض���
void *timer_Q[TIMER_Q_LEN];				//ʱ�䶨ʱ������
void *weather_Q[WEATHER_Q_LEN];			//��ӡ�������
void *RemoteRec_Q[REMOTEREC_Q_LEN];		//�����������
void *Sensor_Data_Collect_Q[SENSOR_DATA_COLLECT_Q_LEN];//�������ɼ�����

//�ߵ�����ͷ��������
u8 CurrentPtzSetCom;
u8 GTZMHDCmdSetStep1;
u8 GTZMHDCmdSetStep2;
u8 GTZMHDCmdSetOk;
u8 DoGTZMHDCmdSetRet;
u8 GTZMHDCmdGetStep1;
u8 GTZMHDCmdGetStep2;
u8 GTZMHDCmdGetOk;
u8 DoGTZMHDCmdGetRet;





/* Private function prototypes -----------------------------------------------*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
extern void  App_OSViewTaskCreate   (void);
#endif

static  void  App_TaskStart			(void	*pdata);  
extern  void  App_TaskCreate        (void);

INT32S main(void)
{ 
 
	CPU_INT08U  os_err;
	os_err = os_err; 
   __disable_irq();	// �ر�ȫ���жϣ�ucosiiҪ������ȹر�ȫ���ж�
	SystemInit();
	
	delay_init(168);		  //��ʼ����ʱ����
	
	OSInit();   
 	
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
														 
	return (0);			
														 
}

 //��ʼ����
static void App_TaskStart(void *pdata)
{
  uint8 err;
	CPU_INT08U  os_err;
	os_err = os_err; 
	pdata = pdata;
	
	NVIC_SetPriority(SysTick_IRQn, 0x0);
	__enable_irq();	
	
	#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                            /* Determine CPU capacity.                              */
	#endif
	
	/* Create application tasks.                               */
	#if (OS_VIEW_MODULE == DEF_ENABLED)
		App_OSViewTaskCreate();
	#endif
	
	//����Ӳ����ʼ��
	bsp_Board_Init();

	//LED2(LED_ON);
	log_sem 	= OSSemCreate(1);
	

	strcpy(gRbtState.RobotName, "Rbt9999");
	gSlaveMtAnaly.s_SlvMtState = SLAVE_MT_CTRL_HANDLE ;
	gRbtState.bRf433Mode3 =false;
	gRbtState.StateSwitchFlag[0] = 'I';	//Ĭ���ֶ�ģʽ��ͨ����ָ���л�
		
	//�����ڴ�ռ�
  p_msgBuffer = OSMemCreate(msgBuffer,100,sizeof(DATA_CONTROL_BLOCK),&err);

	mem160ptr	= OSMemCreate(mem_160,MEM160_NUM,220,&err);
	
	mem512_sem  = OSSemCreate(MEM512_NUM);
	mem512ptr   = OSMemCreate(mem_512,MEM512_NUM,512,&err);	

	//�������
  GetWeatherQueue = OSQCreate(&weather_Q[0],WEATHER_Q_LEN);
	RemoteRecQueue = OSQCreate(&RemoteRec_Q[0],TIMER_Q_LEN);
	timerQueue = OSQCreate(&timer_Q[0],TIMER_Q_LEN);
	LocalQueue = OSQCreate(&local_Q[0],LOCAL_LENGH);
	//Sensor_Data_Collect_Queue = OSQCreate(&Sensor_Data_Collect_Q[0],SENSOR_DATA_LENGH);
	
  PtzSem = OSSemCreate(1);
	
	App_TaskCreate();
	OSTimeDly(OS_TICKS_PER_SEC);
	
	
  //UCOS�汾��
	OSVersion();
	debug_sprintf(ID_DEBUG,"\r\n");
	debug_sprintf(ID_DEBUG,"UCOS�汾��:");
	nprintf(ID_DEBUG,OS_VERSION,0,DEC);
	debug_sprintf(ID_DEBUG,"\r\n");
	
					
}

//����Ѳ������˵ľ�������
static void  App_TaskCreate (void)
{
   CPU_INT08U  os_err;

	 os_err = os_err; 

	//OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)
	//LED2(LED_ON);
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

	//OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)

}	




