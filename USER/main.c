
#include <includes.h>
#include "stm32f4xx.h"                  // Device header file

OS_EVENT	*log_sem;

OS_EVENT	*PtzSem;


DATA_CONTROL_BLOCK 		msgBuffer[100];
OS_MEM								*p_msgBuffer;					//OS_MEM：内存分区数据结构

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


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 起始任务
//设置任务优先级
//#define APP_TASK_START_PRIO      			0 //开始任务的优先级设置为最高
//设置任务堆栈大小
//#define APP_TASK_START_STK_SIZE  				128
//起始任务 任务堆栈	
static OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
//起始任务函数
void App_TaskStart(void *pdata);	
 	
//设置任务优先级
//#define WEATHER_TASK_PRIO      			3
//设置任务堆栈大小
//#define WEATHER_LENGH								100	
//打印输出任务 任务堆栈	
OS_STK TaskStk_Weather[WEATHER_LENGH];
//打印输出任务函数
void Task_Weather(void *pdata);

//设置任务优先级
//#define REMOTE_REC_TASK_PRIO      			5
//设置任务堆栈大小
//#define REMOTE_REC_LENGH								200	
//命令解析任务 任务堆栈	
 OS_STK TaskStk_Remote_Rec[REMOTE_REC_LENGH];
//命令解析任务函数
void Task_Remote_Rec(void *pdata);

//设置任务优先级
//#define TIMER_TASK_PRIO      			7
//设置任务堆栈大小
//#define TIMER_LENGH								100	
//时间定时器任务堆栈	
 OS_STK TaskStk_Timer[TIMER_LENGH];
//时间定时器任务函数
void Task_Timer(void *pdata);

//设置任务优先级
//#define LOCAL_TASK_PRIO      			9
//设置任务堆栈大小
//#define LOCAL_LENGH								200	
//本地任务 任务堆栈	
 OS_STK TaskStk_Local[LOCAL_LENGH];
//本地任务函数
void Task_Local(void *pdata);

//设置任务优先级
//#define SENSOR_DATA_TASK_PRIO      			11
//设置任务堆栈大小
//#define SENSOR_DATA_LENGH								100	
//传感器采集任务任务 任务堆栈	
 OS_STK TaskStk_Sensor_Data_Collect[SENSOR_DATA_LENGH];
//传感器采集任务函数
void Task_Sensor_Data_Collect(void *pdata);	
 
////任务堆栈空间分配
//static  OS_STK     App_TaskStartStk[APP_TASK_START_STK_SIZE]; //起始任务堆栈
//static  OS_STK		 TaskStk_Weather [WEATHER_LENGH];			//打印输出任务堆栈
//static  OS_STK		 TaskStk_Remote_Rec [REMOTE_REC_LENGH];		//命令解析任务堆栈
//static  OS_STK		 TaskStk_Timer [TIMER_LENGH];				//时间定时器任务堆栈
//static  OS_STK		 TaskStk_Local [LOCAL_LENGH];				//本地任务堆栈
//static  OS_STK	 TaskStk_Sensor_Data_Collect[SENSOR_DATA_LENGH];//传感器采集任务堆栈

//队列长度
#define 	REMOTEREC_Q_LEN		32
#define 	LOCAL_Q_LEN				10
#define		WEATHER_Q_LEN			32
#define		TIMER_Q_LEN				32
#define		SENSOR_DATA_COLLECT_Q_LEN	32

void *local_Q[LOCAL_Q_LEN]; 			//本地队列
void *timer_Q[TIMER_Q_LEN];				//时间定时器队列
void *weather_Q[WEATHER_Q_LEN];			//打印输出队列
void *RemoteRec_Q[REMOTEREC_Q_LEN];		//命令解析队列
void *Sensor_Data_Collect_Q[SENSOR_DATA_COLLECT_Q_LEN];//传感器采集队列

//高德摄像头参数定义
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
   __disable_irq();	// 关闭全局中断，ucosii要求必须先关闭全局中断
	SystemInit();
	
	delay_init(168);		  //初始化延时函数
	
	OSInit();   
 	
	//创建起始任务
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

 //起始任务
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
	
	//板载硬件初始化
	bsp_Board_Init();

	//LED2(LED_ON);
	log_sem 	= OSSemCreate(1);
	

	strcpy(gRbtState.RobotName, "Rbt9999");
	gSlaveMtAnaly.s_SlvMtState = SLAVE_MT_CTRL_HANDLE ;
	gRbtState.bRf433Mode3 =false;
	gRbtState.StateSwitchFlag[0] = 'I';	//默认手动模式，通过发指令切换
		
	//分配内存空间
  p_msgBuffer = OSMemCreate(msgBuffer,100,sizeof(DATA_CONTROL_BLOCK),&err);

	mem160ptr	= OSMemCreate(mem_160,MEM160_NUM,220,&err);
	
	mem512_sem  = OSSemCreate(MEM512_NUM);
	mem512ptr   = OSMemCreate(mem_512,MEM512_NUM,512,&err);	

	//队列相关
  GetWeatherQueue = OSQCreate(&weather_Q[0],WEATHER_Q_LEN);
	RemoteRecQueue = OSQCreate(&RemoteRec_Q[0],TIMER_Q_LEN);
	timerQueue = OSQCreate(&timer_Q[0],TIMER_Q_LEN);
	LocalQueue = OSQCreate(&local_Q[0],LOCAL_LENGH);
	//Sensor_Data_Collect_Queue = OSQCreate(&Sensor_Data_Collect_Q[0],SENSOR_DATA_LENGH);
	
  PtzSem = OSSemCreate(1);
	
	App_TaskCreate();
	OSTimeDly(OS_TICKS_PER_SEC);
	
	
  //UCOS版本号
	OSVersion();
	debug_sprintf(ID_DEBUG,"\r\n");
	debug_sprintf(ID_DEBUG,"UCOS版本号:");
	nprintf(ID_DEBUG,OS_VERSION,0,DEC);
	debug_sprintf(ID_DEBUG,"\r\n");
	
					
}

//创建巡检机器人的具体任务
static void  App_TaskCreate (void)
{
   CPU_INT08U  os_err;

	 os_err = os_err; 

	//OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)
	//LED2(LED_ON);
	//打印输出任务	 优先级3
    os_err = OSTaskCreateExt((void (*)(void *)) Task_Weather, 		//任务代码的指针	
														(void 		 * ) 0, 						//当任务开始执行时传递给任务的参数的指针
														(OS_STK		 * )&TaskStk_Weather[WEATHER_LENGH - 1],	//分配给任务的堆栈的栈顶指针	
														(INT8U		   ) WEATHER_TASK_PRIO,						//分配给任务的优先级
														(INT16U          ) WEATHER_TASK_ID,						//为要建立的任务创建一个特殊的标识符
														(OS_STK        * )&TaskStk_Weather[0],					//指向任务的堆栈栈底的指针，用于堆栈的检验
														(INT32U          ) WEATHER_LENGH,						//用于指定堆栈成员数目的容量,用于堆栈的检验
														(void          * )0,									//指向用户附加的数据域的指针
														(INT16U         )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));	//指定是否允许堆栈检验，是否将堆栈清零，任务是否要进行浮点操作等等						
		
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(WEATHER_TASK_PRIO, "Task_Weather", &os_err);
	#endif			
		OSTimeDly(OS_TICKS_PER_SEC);	
		
//命令解析任务  优先级5
   os_err = OSTaskCreateExt((void (*)(void *)) Task_Remote_Rec, 		//任务代码的指针	
													(void 		 * ) 0, 						//当任务开始执行时传递给任务的参数的指针
													(OS_STK		 * )&TaskStk_Remote_Rec[REMOTE_REC_LENGH - 1],	//分配给任务的堆栈的栈顶指针	
													(INT8U		   ) REMOTE_REC_TASK_PRIO,						//分配给任务的优先级
													(INT16U        ) REMOTE_REC_TASK_ID,						//为要建立的任务创建一个特殊的标识符
													(OS_STK      * )&TaskStk_Remote_Rec[0],						//指向任务的堆栈栈底的指针，用于堆栈的检验
													(INT32U        ) REMOTE_REC_LENGH,							//用于指定堆栈成员数目的容量,用于堆栈的检验
													(void        * )0,											//指向用户附加的数据域的指针
													(INT16U        )(OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR)	);	//指定是否允许堆栈检验，是否将堆栈清零，任务是否要进行浮点操作等等						
		
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(REMOTE_REC_TASK_PRIO, "Task_Remote_Rec", &os_err);
	#endif	
				
	//时间 定时器任务  优先级7
    os_err = OSTaskCreateExt (Task_Timer,				//任务代码的指针
					(void *)0,            				//当任务开始执行时传递给任务的参数的指针
					&TaskStk_Timer[TIMER_LENGH - 1],	//分配给任务的堆栈的栈顶指针
					TIMER_TASK_PRIO,					//分配给任务的优先级
					TIMER_TASK_ID,						//为要建立的任务创建一个特殊的标识符
					&TaskStk_Timer[0],					//指向任务的堆栈栈底的指针，用于堆栈的检验
					TIMER_LENGH,						//用于指定堆栈成员数目的容量,用于堆栈的检验
					(void *)0,							//指向用户附加的数据域的指针
					OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR//指定是否允许堆栈检验，是否将堆栈清零，任务是否要进行浮点操作等等
					);
	#if OS_TASK_NAME_EN > 0
		OSTaskNameSet(TIMER_TASK_PRIO, "Task_Timer", &os_err);
	#endif						
	 
   
	 //本地任务				优先级9																									
	 os_err =OSTaskCreateExt(Task_Local,					//任务代码的指针
					(void *)0,            					//当任务开始执行时传递给任务的参数的指针
					&TaskStk_Local[LOCAL_LENGH - 1],		//分配给任务的堆栈的栈顶指针
					LOCAL_TASK_PRIO,						//分配给任务的优先级
					LOCAL_TASK_ID,							//为要建立的任务创建一个特殊的标识符
					&TaskStk_Local[0],						//指向任务的堆栈栈底的指针，用于堆栈的检验
					LOCAL_LENGH,							//用于指定堆栈成员数目的容量,用于堆栈的检验
					(void *)0,								//指向用户附加的数据域的指针
					OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR		//指定是否允许堆栈检验，是否将堆栈清零，任务是否要进行浮点操作等等
					);
  #if OS_TASK_NAME_EN > 0
		OSTaskNameSet(LOCAL_TASK_PRIO, "Task_Local", &os_err);
	#endif
		
	//传感器数据采集任务 优先级11
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

	//OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)

}	




