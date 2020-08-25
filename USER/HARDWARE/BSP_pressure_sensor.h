
#ifndef  	_BSP_PRESSURE_SENSOR_H_
#define 	_BSP_PRESSURE_SENSOR_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"

/*****************************************************/
//压力传感器参数配置

#define		IN_VOLTAGE_MAX						3000		//单位mv //单片机采集最大电压
#define 	PRESSURE_SENSOR_SPAN_MAX	300			//最大量程 重量	牛顿 N

/*****************************************************/

//引脚定义
/*******************************************************/
#define PRESSURE_SENSOR_GPIO_PORT		GPIOC
#define PRESSURE_SENSOR_GPIO_PIN		GPIO_Pin_2
#define PRESSURE_SENSOR_GPIO_CLK		RCC_AHB1Periph_GPIOC

#define	PRESSURE_SENSOR_ADC_CHANNEL     ADC_Channel_12	

#define PRESSURE_SENSOR_ADC_INDEX		2

void bsp_Pressure_Sensor_Init(void);							//初始化
float Pressure_Sensor_Get_Voltage(void);						//获取电压
float bsp_Pressure_Sensor_Get_Newton(void);							//获取压力值，单位牛顿

#endif
