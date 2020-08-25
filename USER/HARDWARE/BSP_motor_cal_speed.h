
#ifndef  	_BSP_MOTOR_CAL_SPEED_H_
#define 	_BSP_MOTOR_CAL_SPEED_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"

/*****************************************************/
//主电机模拟速度计算
#define		MASTER_MOTOR_IN_VOLTAGE_MAX				3000					//单位mv //单片机采集最大电压
#define 	MASTER_MOTOR_SPEED_SPAN_MAX				8000		  		//主电机最大转速 	单位 转 

//从电机模拟速度计算

#define		SLAVE_MOTOR_IN_VOLTAGE_MAX				3000					//单位mv //单片机采集最大电压
#define 	SLAVE_MOTOR_SPEED_SPAN_MAX				10000		  		//从电机最大转速 	单位 转 


/*****************************************************/

//主电机模拟速度引脚定义
/*******************************************************/
#define MASTER_MOTOR_SPEED_GPIO_PORT			GPIOC
#define MASTER_MOTOR_SPEED_GPIO_PIN				GPIO_Pin_1
#define MASTER_MOTOR_SPEED_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define	MASTER_MOTOR_SPEED_ADC_CHANNEL    ADC_Channel_11	
#define MASTER_MOTOR_SPEED_ADC_INDEX			1

//从电机模拟速度引脚定义
/*******************************************************/
#define SLAVE_MOTOR_SPEED_GPIO_PORT				GPIOC
#define SLAVE_MOTOR_SPEED_GPIO_PIN				GPIO_Pin_0
#define SLAVE_MOTOR_SPEED_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define	SLAVE_MOTOR_SPEED_ADC_CHANNEL     ADC_Channel_10	
#define SLAVE_MOTOR_SPEED_ADC_INDEX				0



void bsp_master_cal_speed_Init(void);							//主动轮模拟速度计算初始化

void bsp_slave_cal_speed_Init(void);							//从动轮模拟速度计算初始化

float Master_Cal_Speed_Get_Voltage(void);
	
float Slave_Cal_Speed_Get_Voltage(void);

int  bsp_master_voltage_get_speed(void);

int bsp_slave_voltage_get_speed(void);

#endif



