
#ifndef  	_BSP_MOTOR_CAL_SPEED_H_
#define 	_BSP_MOTOR_CAL_SPEED_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"

/*****************************************************/
//�����ģ���ٶȼ���
#define		MASTER_MOTOR_IN_VOLTAGE_MAX				3000					//��λmv //��Ƭ���ɼ�����ѹ
#define 	MASTER_MOTOR_SPEED_SPAN_MAX				8000		  		//��������ת�� 	��λ ת 

//�ӵ��ģ���ٶȼ���

#define		SLAVE_MOTOR_IN_VOLTAGE_MAX				3000					//��λmv //��Ƭ���ɼ�����ѹ
#define 	SLAVE_MOTOR_SPEED_SPAN_MAX				10000		  		//�ӵ�����ת�� 	��λ ת 


/*****************************************************/

//�����ģ���ٶ����Ŷ���
/*******************************************************/
#define MASTER_MOTOR_SPEED_GPIO_PORT			GPIOC
#define MASTER_MOTOR_SPEED_GPIO_PIN				GPIO_Pin_1
#define MASTER_MOTOR_SPEED_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define	MASTER_MOTOR_SPEED_ADC_CHANNEL    ADC_Channel_11	
#define MASTER_MOTOR_SPEED_ADC_INDEX			1

//�ӵ��ģ���ٶ����Ŷ���
/*******************************************************/
#define SLAVE_MOTOR_SPEED_GPIO_PORT				GPIOC
#define SLAVE_MOTOR_SPEED_GPIO_PIN				GPIO_Pin_0
#define SLAVE_MOTOR_SPEED_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define	SLAVE_MOTOR_SPEED_ADC_CHANNEL     ADC_Channel_10	
#define SLAVE_MOTOR_SPEED_ADC_INDEX				0



void bsp_master_cal_speed_Init(void);							//������ģ���ٶȼ����ʼ��

void bsp_slave_cal_speed_Init(void);							//�Ӷ���ģ���ٶȼ����ʼ��

float Master_Cal_Speed_Get_Voltage(void);
	
float Slave_Cal_Speed_Get_Voltage(void);

int  bsp_master_voltage_get_speed(void);

int bsp_slave_voltage_get_speed(void);

#endif



