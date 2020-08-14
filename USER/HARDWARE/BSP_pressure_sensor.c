#include  "bsp_pressure_sensor.h"

/**
  ******************************************************************************
  * @file    bsp_pressure_sensor.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   ѹ������������
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
  * @brief  ѹ����������ʼ����������λ������ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/
void bsp_Pressure_Sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(PRESSURE_SENSOR_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = PRESSURE_SENSOR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(PRESSURE_SENSOR_GPIO_PORT, &GPIO_InitStructure);			
}

/*
  * @brief  ��ȡѹ����������ѹֵ
  * @param  ��
  * @retval ���ص�ѹֵ����λmv��
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
  * @brief  ��ȡ��ǰ���� 
  * @param  ��
  * @retval ��������ֵ����λg��
*/

float bsp_Pressure_Sensor_Get_Weight(void)
{
	float Voltage;
	Voltage = Pressure_Sensor_Get_Voltage();
	return Voltage/(IN_VOLTAGE_MAX)*PRESSURE_SENSOR_SPAN_MAX;
}

