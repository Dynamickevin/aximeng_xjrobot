
#include "includes.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 起始任务
//设置任务优先级
//#define APP_TASK_START_PRIO      			0 //开始任务的优先级设置为最低
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




int main(void)
{ 
 
	CPU_INT08U  os_err;
	os_err = os_err; 
   __disable_irq();	// 关闭全局中断，ucosii要求必须先关闭全局中断
	SystemInit();
	delay_init(168);		  //初始化延时函数
	bsp_Led_Init();		    //初始化LED端口
	
	OSInit();   
 	//OSTaskCreate(App_TaskStart,(void *)0,(OS_STK *)&App_TaskStartStk[APP_TASK_START_STK_SIZE-1],APP_TASK_START_PRIO );//创建起始任务
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
}

 //开始任务
void App_TaskStart(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	CPU_INT08U  os_err;
	os_err = os_err; 
	pdata = pdata;
	
  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)
	
 	//OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	//OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
	
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

	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
					
} 




