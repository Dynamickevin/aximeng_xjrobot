#include  "bsp_pressure_sensor.h"

/**
  ******************************************************************************
  * @file    bsp_pressure_sensor.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   压力传感器驱动
  ******************************************************************************
  * @attention
  *		
  * 	
  *		
  *		
  *		
  *
  ******************************************************************************
  */

 /*
  * @brief  压力传感器初始化函数，限位开关在使用前必须先调用此函数
  * @param  无
  * @retval 无
*/
void bsp_Pressure_Sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(PRESSURE_SENSOR_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = PRESSURE_SENSOR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(PRESSURE_SENSOR_GPIO_PORT, &GPIO_InitStructure);			
}

/*
  * @brief  获取压力传感器电压值
  * @param  无
  * @retval 返回电压值（单位mv）
*/
float Pressure_Sensor_Get_Voltage(void)
{
	u8 i;
	u32 Adc_Sum = 0;
	float tmp;
	for(i = 0 ; i < ADC_BUFFSIZE/ADC_CHANNEL_NUM ; i++){
		Adc_Sum += ADC_Value[PRESSURE_SENSOR_ADC_INDEX + ADC_CHANNEL_NUM*i];
	}
	tmp = Adc_Sum/(ADC_BUFFSIZE/ADC_CHANNEL_NUM) *3300/4096;
	return tmp;
}



/*
  * @brief  获取当前重量 
  * @param  无
  * @retval 返回重量值（单位g）
*/

float bsp_Pressure_Sensor_Get_Weight(void)
{
	float Voltage;
	Voltage = Pressure_Sensor_Get_Voltage();
	return Voltage/(IN_VOLTAGE_MAX)*PRESSURE_SENSOR_SPAN_MAX;
}

