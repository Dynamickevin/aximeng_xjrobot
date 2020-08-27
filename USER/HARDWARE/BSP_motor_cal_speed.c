
#include "BSP_motor_cal_speed.h"


/*
  * @brief  主动轮模拟速度计算初始化函数
  * @param  无
  * @retval 无
*/
void bsp_master_cal_speed_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(MASTER_MOTOR_SPEED_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = MASTER_MOTOR_SPEED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(MASTER_MOTOR_SPEED_GPIO_PORT, &GPIO_InitStructure);		
	
}



/*
  * @brief  //从动轮模拟速度计算初始化函数
  * @param  无
  * @retval 无
*/
void bsp_slave_cal_speed_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(SLAVE_MOTOR_SPEED_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = SLAVE_MOTOR_SPEED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(SLAVE_MOTOR_SPEED_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
  * @brief  获取主动轮模拟速度电压值
  * @param  无
  * @retval 返回电压值（单位mv）
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
  * @brief  获取从动轮模拟速度电压值
  * @param  无
  * @retval 返回速度值
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
  * @brief  获取当前主电机速度值 
  * @param  无
  * @retval 返回速度值（单位转）
*/

int bsp_master_voltage_get_speed(void)
{
	int Master_Voltage;
	Master_Voltage = Master_Cal_Speed_Get_Voltage();
	return Master_Voltage/(MASTER_MOTOR_IN_VOLTAGE_MAX)*MASTER_MOTOR_SPEED_SPAN_MAX;
	
}

/*
  * @brief  获取当前从电机速度值 
  * @param  无
  * @retval 返回速度值（单位转）
*/

int bsp_slave_voltage_get_speed(void)
{
	int Slave_Voltage;
	Slave_Voltage = Slave_Cal_Speed_Get_Voltage();
	return Slave_Voltage/(SLAVE_MOTOR_IN_VOLTAGE_MAX)*SLAVE_MOTOR_SPEED_SPAN_MAX;
	
}




