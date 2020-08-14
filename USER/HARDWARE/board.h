#ifndef _BOARD_H_
#define _BOARD_H_


#include "NVIC_IRQ.h"
#include "BSP_led.h"
#include "bsp_delay.h" 
#include "bsp_pressure_sensor.h"
#include "BSP_limit_SW.h"
#include "BSP_MOTOR.h"
#include "BSP_GPS.h"
#include "BSP_enccoder.h"
#include "bsp_general_tim.h"
#include "BSP_GPS.h"
#include  "BSP_HI3521.h"
#include  "BSP_AM2320.h"
#include "BSP_BRD.h"
#include "BSP_UART.h"
#include "BSP_Video_TRANS.h"
#include "BSP_RF.h"


void bsp_Board_Init(void);
int Robot_Demo(void);
void Get_Info(void);
void STM_TX_Robot_Info(void);
#endif


/**************************************************END LINE**********************************************/
