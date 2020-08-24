#ifndef 	_BSP_ENCCODER_H_
#define 	_BSP_ENCCODER_H_



#include "stm32f4xx.h"



//Òý½Å¶¨Òå
/*******************************************************/
#define ENCCODER_A_PIN                  GPIO_Pin_0               
#define ENCCODER_A_GPIO_PORT            GPIOA                     
#define ENCCODER_A_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define	ENCCODER_A_AF                   GPIO_AF_TIM5
#define ENCCODER_A_SOURCE               GPIO_PinSource0
	
#define ENCCODER_B_PIN                  GPIO_Pin_1                 
#define ENCCODER_B_GPIO_PORT            GPIOA                      
#define ENCCODER_B_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define ENCCODER_B_AF                   GPIO_AF_TIM5
#define ENCCODER_B_SOURCE               GPIO_PinSource1

#define ENCCODER_Z_PIN                  GPIO_Pin_5               
#define ENCCODER_Z_GPIO_PORT            GPIOA                 
#define ENCCODER_Z_GPIO_CLK             RCC_AHB1Periph_GPIOA

#define	ENCCODER_Z_EXTI_PORT			EXTI_PortSourceGPIOA
#define	ENCCODER_Z_EXTI_PIN				EXTI_PinSource5
#define	ENCCODER_Z_EXTI_LINE			EXTI_Line5
#define	ENCCODER_Z_IRQn						EXTI9_5_IRQn



#define ENCCODER_AB_TIM           		TIM5
#define ENCCODER_AB_TIM_CLK       		RCC_APB1Periph_TIM5

#define ENCCODER_AB_TIM_IRQn					TIM5_IRQn
#define ENCCODER_AB_IRQHandler    		TIM5_IRQHandler



/************************************************************/



/**************************************************************/

/**************************************************************/

	
void ENCCODER_Z_EXTI_Handler(void);
void  bsp_enccoder_Init(void);
int32_t bsp_Enccoder_AB_GET_Cnt(void);
					
#endif

