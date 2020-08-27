/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               includes.h
** Descriptions:            includes.h for ucos
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             kevin
** Modified date:          2019-04-04 
** Version:                 
** Descriptions:           Inspection Robot 
**
*********************************************************************************************************/


#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include  <UCOSII\CORE\ucos_ii.h>
#include  <UCOSII\PORT\os_cpu.h>

#include  <uC-CPU\cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#if (OS_VIEW_MODULE == DEF_ENABLED)
#include   <uCOS-VIEW\Ports\ARM-Cortex-M4\Realview\os_viewc.h>
#include   <uCOS-VIEW\Source\os_view.h>
#endif

#include  <stm32f4xx.h>
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"


#define Q_MAX_SIZE		2
extern void Delay(u32 nCount);


typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

typedef struct {
    uint8  type;
	//uint8  id;
    uint16 count;
    uint8 *point;
}DATA_CONTROL_BLOCK;

//�ź���
#define BATTERY_MSG          1
#define SENSOR_MSG           2
#define MOTOR_MSG            3
#define WHL_MSG              8
#define AM2320_MSG           10
#define SYS_LED_MSG          11
#define SEN_DATA_MSG         12


#define WEATHER_DEBUG_OUT_MSG_DEBUG   			 0xf1
#define WEATHER_DEBUG_OUT_MSG_LINUX   			 0xf2
#define WEATHER_DEBUG_OUT_MSG_RF433    			 0xf3
#define WEATHER_DEBUG_OUT_MSG_POWER_BOARD    0xf4

#define LOCAL_DEBUG_IN_MSG_DEBUG       				0x81
#define LOCAL_DEBUG_IN_MSG_LINUX      			 	0x82
#define LOCAL_DEBUG_IN_MSG_RF433      			 	0x83
#define LOCAL_DEBUG_IN_MSG_POWER_BOARD       	0x84

//����ʶ���
#define ID_RF433               				1
#define ID_POWER_BOARD                2               
#define ID_LINUX               			  3
#define ID_DEBUG               			  4

//�������߹رյ�����ܲ���
//����������ܲ��Ժ���������Ӷ��ֿ��Ʒ�ʽ�޸�Ϊ���ⲿ�������
//�����ٶȣ����ֱ����������ٶ�
#define MOTER_PERFORM_TEST_EN   1

#define YES 0x55
#define NOT 0xaa

#define OS_NO_ERR               0

#define true  1
#define false 0


#ifndef bool
#define bool unsigned char
#endif

#define LsiFreq 32768

//-----------------����ϵͳ�ڴ�鶨��----------------------

#define		MEM64_NUM 			12		//1K
#define		MEM160_NUM 			30		//1K          
#define		MEM256_NUM 			5		//4K
#define		MEM512_NUM 			7		//3K 
#define		MEM1024_NUM			4		//3K


//-----------------������������ȼ�����----------------------

#define		WEATHER_TASK_PRIO				3
#define		WEATHER_TASK_ID					3

#define		REMOTE_REC_TASK_PRIO		5
#define		REMOTE_REC_TASK_ID			5

#define		TIMER_TASK_PRIO	   			7
#define		TIMER_TASK_ID						7

#define		LOCAL_TASK_PRIO					9
#define		LOCAL_TASK_ID						9

#define		SENSOR_DATA_TASK_PRIO		11
#define		SENSOR_DATA_ID					11


//-----------------��������������ջ����----------------------

#define		START_LENGH			100		//��ʼ����
#define		WEATHER_LENGH		100		//��ӡ�������	
#define		TIMER_LENGH			100		//ʱ�� ��ʱ������
#define		LOCAL_LENGH			200		//��������
#define		REMOTE_REC_LENGH	200		//������ս�������
//#define		REMOTE_LENGH	100		//Զ������
#define		SENSOR_DATA_LENGH	100		//�������ɼ�����


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_DISABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


//-----------------���ⲿ�����ı���----------------------
extern OS_EVENT  	*log_sem;

extern OS_EVENT *Semaphore1;
extern OS_EVENT	*Mbox1;


extern OS_MEM		*p_msgBuffer;

extern OS_MEM		*mem160ptr;
extern INT8U		 mem_160[MEM160_NUM][220];

extern OS_EVENT 	*mem512_sem;
extern OS_MEM		*mem512ptr;
extern INT8U		 mem_512[MEM512_NUM][512];

//#define MCU_RAM_ADDR_BEGIN         0x20000000
//#define MCU_RAM_ADDR_USER_BEGIN   (0x20000000+0x1000) //4K  �û�RAM�������Сֵ
//#define MCU_RAM_ADDR_END          (0x20000000+0x30000) //192K

#define MCU_FLASH_ADDR_BEGIN       0x8000000 
#define MCU_FLASH_ADDR_END        (0x8000000+0x5FFFF) //  FLASH_Sector_6 128K


#define BOX_OS_USER_SLV_CFG_ADDR (MCU_FLASH_ADDR_BEGIN + 0x40000) //  FLASH_Sector_6



//���й��ڻ����˵����ݶ�����ڸýṹ��
typedef struct{
    u8		sendbuf[1000];		//���ͻ�����
    u16     counter;
    u16		icmd_len;			//�����
    int     trans_vals[6];      //���ڸ����������ݵ�����
    //���������״̬
    u8 nTimeForNoLinuxHeartIn ; //û������������
}ZT_INFO_TYPE;



//////////////////////////////////////////////////////////////////////////
//����ͬ���ڽ��յ�ATָ�����ݣ�����ṹ���¼������ռ�����״̬
typedef struct{
    unsigned char Data;
	unsigned char CurSendStatus;
	unsigned char CurRecStatus;
    //UartDriverCtrl* uart;  //AT����ʹ�õĴ���
    //const char*     name;  //ATָ����Դ����
    u8 recv_buf[254];
    u8 head[2];
	u8 counter;
    u8 recv_len;
    //bool bRecvdOneCmd;  //�����ֵΪtrue����ʾ�Ѿ����ճɹ�һ�����ݰ���
}AtCmdInfo;

typedef struct{
    bool bCX_SwCheck1 ;  //��Ѩ��λ����1
    bool bCX_SwCheck2 ;  //��Ѩ��λ����2
    bool bChargeVolIn ;  //���ӿ��е�ѹ����
    bool bChargeShort ;  //���ӿ��ⲿ��·
    u16  nBatVol  ;      //��ص�ѹ�˲���ֵ  ��Ҫ����ϵ��  21.0f*2.5f/4096 ��Ϊ������ص�ѹ
    u16  nBatCur  ;      //��ص����˲���ֵ ��Ҫ����ϵ�� (2.5f/4096/0.3f) 
    char GpsPosState[36];  //GPS λ���ַ��� 116.1479415,33.7518119,61.2
    char GpsTimeState[24]; //17,11,20,14,0,24
    char RobotName[24];		//����������
    char RobotState[28];   //����״̬��Idle���ֶ�����״̬��Handle���Զ�Ѳ��״̬��AutoRun�����س��״̬��BackChg��
    bool bRf433Mode3;
	char StateSwitchFlag[8];  //�Զ�Ѳ��ʱ�����յ��ֶ�ָ�״̬�л���H:�ֶ�״̬��A���Զ�״̬��C�����س��
}RbtState;

#define CMD_PARA_STRING_LEN_NAME  20
#define CMD_PARA_STRING_LEN_VALUE 40

#pragma pack(1)
typedef struct{//__packed 
	char name [CMD_PARA_STRING_LEN_NAME];					//��������
	char value[CMD_PARA_STRING_LEN_VALUE];					//����ֵ����
    //int item_begin_pos; //��ǰ ���������� �����������еĿ�ʼλ��
    char item_begin_pos;
	//int name_str_len;
	char name_str_len;
    //int value_str_len;
	char value_str_len;
}CmdParaOneItem;

typedef struct
{
    CmdParaOneItem items[30];
    int            item_cnt;
}CmdParaInfo;


//������901
/*typedef struct
{
    s16 accl[3];        //���ٶ�ֵ
    s16 angle_vel[3];   //���ٶ�
    s16 angle[3];       //�Ƕ�
    s16 magnetic[3];    //�ų�����
    s16 temperature;    //�¶�
}jy_901_data_type;
extern jy_901_data_type gJy901Data;*/

typedef struct{
    bool bHongWai;  		//0-�ɼ���Ƶ  1-������Ƶ
    u8   lastCmd;
    u16  nInfraredFocalDis; //���⽹��
    u16  nVisibleFocalDis;  //�ɼ��⽹��
    s16  directAngle;  		//��λ��
    s16  pitchAngle;   		//������
    s16  rollAngle;    		//�����   
}CamSet; 
extern CamSet GTZMHD_Set;

typedef struct{
    bool bHongWai;  		 //0-�ɼ���Ƶ  1-������Ƶ
    u8   lastCmd;
    u16  nInfraredFocalDis;  //���⽹��
    u16  nVisibleFocalDis;   //�ɼ��⽹��
    s16  directAngle;  		 //��λ��
    s16  pitchAngle;  		 //������
    s16  rollAngle;    		 //�����   
}CamGet; 
extern CamGet GTZMHD_Get;

extern u8 CurrentPtzSetCom;
extern u8 GTZMHDCmdSetStep1;
extern u8 GTZMHDCmdSetStep2;
extern u8 GTZMHDCmdSetOk;
extern u8 DoGTZMHDCmdSetRet;
extern u8 GTZMHDCmdGetStep1;
extern u8 GTZMHDCmdGetStep2;
extern u8 GTZMHDCmdGetOk;
extern u8 DoGTZMHDCmdGetRet;
extern char Frame_No_Debug[4];
extern char Frame_No_Linux[4];
extern char Frame_No_Rf433[4];
extern char Frame_No_POWER[4];
extern signed short CodeAB_Start;
extern char CodeZ_Start;
extern OS_EVENT	*PtzSem;
extern signed long gCodeZ;
extern signed long CodeAB_Last;

extern ZT_INFO_TYPE g_zt_msg;
extern  RbtState gRbtState;



#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif

#define BAT_VOL_FLOAT   (gRbtState.nBatVol*21.0f*2.5f/4096)
#define BAT_CUR_FLOAT   (gRbtState.nBatCur*2.5f/0.3f/4096)

#include "Task_Timer.h"
#include "Task_Local.h"
//#include "Task_Remote.h"
#include "Task_Weather.h"
#include "Task_Remote_Rec.h"
#include "Task_Sensor_Data_Collect.h"


#include "uart_api.h"
#include "zt_motor_master_driver.h"
#include "zt_motor_slave_driver.h"
#include "TimerPWm_Driver.h"
#include "GpioDefine.h"
#include "GpioDriver.h"
#include "user_adc_ctrl.h"
#include "adc_dma.h"
#include "str.h"
#include "cmd_para.h"
#include "App_cfg.h"
#include "AM2320_By_IIC.h"
//#include "Wheel_Encode.h"
//#include "OperationList.h"
//#include "GpsUart.h"
//#include "debug.h"

#include "BSP_enccoder.h"
#include "BSP_motor_cal_speed.h"
#include  "board.h"


#include "sys.h"
#include "delay.h"
#include "usart.h"


#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


