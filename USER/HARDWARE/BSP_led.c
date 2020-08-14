#include "BSP_led.h"

/***
	***********************************************************************
	*							LED驱动程序
	*
	***********************************************************************
***/

/*
  * @brief  Led初始化
  * @param  无
  * @retval 无
*/
void  bsp_Led_Init(void)
{

	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|LED2_GPIO_CLK, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
	
	/*关闭LED1灯*/
	LED1(LED_OFF);	
	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
	/*关闭LED2灯*/
	LED2(LED_OFF);	
	
		/*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = LED_EXT1_PIN;	
	GPIO_Init(LED_EXT1_GPIO_PORT, &GPIO_InitStructure);	
	/*关闭LED2灯*/
	LED_EXT1(LED_OFF);	
	
		/*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = LED_EXT2_PIN;	
	GPIO_Init(LED_EXT2_GPIO_PORT, &GPIO_InitStructure);	
	/*关闭LED2灯*/
	LED_EXT2(LED_OFF);	
	
}
