#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"
#include "includes.h"
#define GENERAL_TIM           		TIM6
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM6
#define GENERAL_TIM_PSC				168/2-1
#define GENERAL_TIM_ARR				1000-1

#define GENERAL_TIM_IRQn			TIM6_DAC_IRQn
#define GENERAL_TIM_IRQHandler    	TIM6_DAC_IRQHandler




void GENERAL_TIM_Configuration(void);

#endif /* __GENERAL_TIM_H */

/**************************************************END LINE**********************************************/
