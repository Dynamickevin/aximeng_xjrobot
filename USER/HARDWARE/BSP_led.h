#ifndef 	_BSP_LED_H_
#define 	_BSP_LED_H_



#include "stm32f4xx.h"

//引脚定义
/*******************************************************/
#define LED1_PIN                  GPIO_Pin_9                 
#define LED1_GPIO_PORT            GPIOF                      
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOF

#define LED2_PIN                  GPIO_Pin_10                
#define LED2_GPIO_PORT            GPIOF                      
#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOF


#define LED_EXT1_PIN                  GPIO_Pin_1                 
#define LED_EXT1_GPIO_PORT            GPIOB                      
#define LED_EXT1_GPIO_CLK             RCC_AHB1Periph_GPIOB

#define LED_EXT2_PIN                  GPIO_Pin_0                 
#define LED_EXT2_GPIO_PORT            GPIOB                      
#define LED_EXT2_GPIO_CLK             RCC_AHB1Periph_GPIOB



/************************************************************/

#define LED_ON  0
#define LED_OFF 1

//LED驱动
/**************************************************************/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN)
					
#define LED_EXT1(a)	if (a)	\
					GPIO_SetBits(LED_EXT1_GPIO_PORT,LED_EXT1_PIN);\
					else		\
					GPIO_ResetBits(LED_EXT1_GPIO_PORT,LED_EXT1_PIN)

#define LED_EXT2(a)	if (a)	\
					GPIO_SetBits(LED_EXT2_GPIO_PORT,LED_EXT2_PIN);\
					else		\
					GPIO_ResetBits(LED_EXT2_GPIO_PORT,LED_EXT2_PIN)
					
//@example	: LED1(LED_ON) led1亮      LED1(LED_OFF) led1灭
/**************************************************************/

	

void  bsp_Led_Init(void);
					
					
#endif

