#include "BSP_led.h"

/***
	***********************************************************************
	*							LED��������
	*
	***********************************************************************
***/

/*
  * @brief  Led��ʼ��
  * @param  ��
  * @retval ��
*/
void  bsp_Led_Init(void)
{

	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|LED2_GPIO_CLK, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
	
	/*�ر�LED1��*/
	LED1(LED_OFF);	
	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
	/*�ر�LED2��*/
	LED2(LED_OFF);	
	
		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStructure.GPIO_Pin = LED_EXT1_PIN;	
	GPIO_Init(LED_EXT1_GPIO_PORT, &GPIO_InitStructure);	
	/*�ر�LED2��*/
	LED_EXT1(LED_OFF);	
	
		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStructure.GPIO_Pin = LED_EXT2_PIN;	
	GPIO_Init(LED_EXT2_GPIO_PORT, &GPIO_InitStructure);	
	/*�ر�LED2��*/
	LED_EXT2(LED_OFF);	
	
}
