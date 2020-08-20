#ifndef  	_BSP_ADC_INIT_H_
#define 	_BSP_ADC_INIT_H_


#include "stm32f4xx.h"

#include "includes.h"

/*****************************************************/

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define ADC_DMA_CLK      	RCC_AHB1Periph_DMA2
#define ADC_DMA_CHANNEL  	DMA_Channel_0
#define ADC_DMA_STREAM   	DMA2_Stream0

#define ADC_USE						ADC1
#define ADC_CLK         	RCC_APB2Periph_ADC1

#define ADC_CHANNEL_NUM		4	
/*******************************************************/

#define ADC_BUFFSIZE	20* ADC_CHANNEL_NUM    //20 平均滤波 个数

extern __IO uint16_t ADC_Value[ADC_BUFFSIZE];

 void bsp_ADC_Init(void);							//初始化
#endif
