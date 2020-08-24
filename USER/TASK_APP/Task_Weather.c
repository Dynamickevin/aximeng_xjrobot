

#define IN_TASK_WEATHER
#include "includes.h"

//---------本任务涉及到的消息队列-----------
OS_EVENT *GetWeatherQueue;			

/******************************************************/

extern OS_MEM		*p_msgBuffer;


//打印输出任务 入口函数
void Task_Weather(void *pdata)
{
	uint8 err;
	DATA_CONTROL_BLOCK *bp;
	
	while ( 1 )
	{	
		bp = (DATA_CONTROL_BLOCK*)OSQPend(GetWeatherQueue,0,&err);
		
		if ( err == OS_ERR_NONE )
		{
			switch(bp->type)
			{
				case WEATHER_DEBUG_OUT_MSG_RF433:
				{
					uart1_send(bp->point, bp->count);
					//LED2(LED_ON);
					OSMemPut(mem160ptr,bp->point);
					break;
				}				
				case WEATHER_DEBUG_OUT_MSG_POWER_BOARD:
				{
					uart2_send(bp->point, bp->count);
					OSMemPut(mem160ptr,bp->point);
					break;
				}				
				case WEATHER_DEBUG_OUT_MSG_LINUX:
				{
					uart3_send(bp->point, bp->count);
					OSMemPut(mem160ptr,bp->point);
					break;
				}	
				case WEATHER_DEBUG_OUT_MSG_DEBUG:
				{
					uart4_send(bp->point, bp->count);
					OSMemPut(mem160ptr,bp->point);
					break;
				}		
				default:
					break;				
			}
	 	}
		OSMemPut(p_msgBuffer,(void *)bp);
	}
}

