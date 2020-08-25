
#ifndef  	_BSP_PRESSURE_SENSOR_H_
#define 	_BSP_PRESSURE_SENSOR_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"

/*****************************************************/
//ѹ����������������

#define		IN_VOLTAGE_MAX						3000		//��λmv //��Ƭ���ɼ�����ѹ
#define 	PRESSURE_SENSOR_SPAN_MAX	300			//������� ����	ţ�� N

/*****************************************************/

//���Ŷ���
/*******************************************************/
#define PRESSURE_SENSOR_GPIO_PORT		GPIOC
#define PRESSURE_SENSOR_GPIO_PIN		GPIO_Pin_2
#define PRESSURE_SENSOR_GPIO_CLK		RCC_AHB1Periph_GPIOC

#define	PRESSURE_SENSOR_ADC_CHANNEL     ADC_Channel_12	

#define PRESSURE_SENSOR_ADC_INDEX		2

void bsp_Pressure_Sensor_Init(void);							//��ʼ��
float Pressure_Sensor_Get_Voltage(void);						//��ȡ��ѹ
float bsp_Pressure_Sensor_Get_Newton(void);							//��ȡѹ��ֵ����λţ��

#endif
