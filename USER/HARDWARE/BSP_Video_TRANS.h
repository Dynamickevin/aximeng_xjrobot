#ifndef 	_BSP_VIDEO_TRANS_H_
#define 	_BSP_VIDEO_TRANS_H_


#include "stm32f4xx.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define VEDIO_TRANS_PWR_EN_PIN                  GPIO_Pin_15          
#define VEDIO_TRANS_PWR_EN_PORT          			  GPIOA                     
#define VEDIO_TRANS_PWR_EN_GPIO_CLK             RCC_AHB1Periph_GPIOA



void bsp_Video_Trans_PWR(FunctionalState NewSta);
void  bsp_Video_Trans_PWR_Init(void);
#endif

