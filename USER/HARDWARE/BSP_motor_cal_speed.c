
#include "BSP_motor_cal_speed.h"


/*
  * @brief  ������ģ���ٶȼ����ʼ������
  * @param  ��
  * @retval ��
*/
void bsp_master_cal_speed_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(MASTER_MOTOR_SPEED_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MASTER_MOTOR_SPEED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(MASTER_MOTOR_SPEED_GPIO_PORT, &GPIO_InitStructure);		
	
}



/*
  * @brief  //�Ӷ���ģ���ٶȼ����ʼ������
  * @param  ��
  * @retval ��
*/
void bsp_slave_cal_speed_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(SLAVE_MOTOR_SPEED_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = SLAVE_MOTOR_SPEED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(SLAVE_MOTOR_SPEED_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
  * @brief  ��ȡ������ģ���ٶȵ�ѹֵ
  * @param  ��
  * @retval ���ص�ѹֵ����λmv��
*/
float Master_Cal_Speed_Get_Voltage(void)
{
	u8 i;
	u32 Adc_Sum = 0;
	float tmp;
	for(i = 0 ; i < ADC_BUFFSIZE / ADC_CHANNEL_NUM ; i++)
	{
		Adc_Sum += ADC_Value[MASTER_MOTOR_SPEED_ADC_INDEX + ADC_CHANNEL_NUM*i];
	}
	tmp = Adc_Sum/(ADC_BUFFSIZE/ADC_CHANNEL_NUM) *3300/4096;
	return tmp;
}

/*
  * @brief  ��ȡ�Ӷ���ģ���ٶȵ�ѹֵ
  * @param  ��
  * @retval �����ٶ�ֵ
*/
float Slave_Cal_Speed_Get_Voltage(void)
{
	u8 i;
	u32 Adc_Sum = 0;
	float tmp;
	for(i = 0 ; i < ADC_BUFFSIZE/ADC_CHANNEL_NUM ; i++)
	{
		Adc_Sum += ADC_Value[SLAVE_MOTOR_SPEED_ADC_INDEX + ADC_CHANNEL_NUM*i];
	}
	tmp = Adc_Sum /(ADC_BUFFSIZE/ADC_CHANNEL_NUM) *3300/4096;
	return tmp;
}

/*
  * @brief  ��ȡ��ǰ������ٶ�ֵ 
  * @param  ��
  * @retval �����ٶ�ֵ����λת��
*/

int bsp_master_voltage_get_speed(void)
{
	int Master_Voltage;
	Master_Voltage = Master_Cal_Speed_Get_Voltage();
	return Master_Voltage/(MASTER_MOTOR_IN_VOLTAGE_MAX)*MASTER_MOTOR_SPEED_SPAN_MAX;
	
}

/*
  * @brief  ��ȡ��ǰ�ӵ���ٶ�ֵ 
  * @param  ��
  * @retval �����ٶ�ֵ����λת��
*/

int bsp_slave_voltage_get_speed(void)
{
	int Slave_Voltage;
	Slave_Voltage = Slave_Cal_Speed_Get_Voltage();
	return Slave_Voltage/(SLAVE_MOTOR_IN_VOLTAGE_MAX)*SLAVE_MOTOR_SPEED_SPAN_MAX;
	
}




