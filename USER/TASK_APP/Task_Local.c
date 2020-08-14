#define IN_TASK_LOCAL
#include "includes.h"

//ZT_INFO_TYPE g_zt_msg;
extern ZT_INFO_TYPE g_zt_msg2;
int  gCmdParaVal_Ints[8];
float gCmdParaVal_floats[3];
extern RbtState gRbtState;
CmdParaInfo gCmdParaInfos;
CamSet GTZMHD_Set;
CamGet GTZMHD_Get;
extern AtCmdInfo AtCmdFromGPS;
char Frame_No_Debug[4];
char Frame_No_Linux[4];
char Frame_No_Rf433[4];

OS_EVENT *LocalQueue;				//本地队列




void Task_Local(void *pdata)
{
	DATA_CONTROL_BLOCK *msg;
	uint8 err;
	
	//OSTimeDly(OS_TICKS_PER_SEC);

	while ( 1 )
	{
		msg = (DATA_CONTROL_BLOCK*)OSQPend(LocalQueue,0,&err);
		
		if ( err == OS_ERR_NONE )
		{
			switch ( msg->type )
			{
				case MOTOR_MSG:
				{
					//LED2(LED_ON);
					//TaskFun_MotorUpdate();
				}
				    break;
				default:
				    break;
			}		
			OSMemPut(p_msgBuffer,(void*)msg);
		}	

	}

}




