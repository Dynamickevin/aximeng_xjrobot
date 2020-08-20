#ifndef  	_BSP_ADC_INIT_H_
#define 	_BSP_ADC_INIT_H_


#include "stm32f4xx.h"

#include "includes.h"

/*****************************************************/

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define ADC_DMA_CLK      	RCC_AHB1Periph_DMA2
#define ADC_DMA_CHANNEL  	DMA_Channel_0
#define ADC_DMA_STREAM   	DMA2_Stream0

#define ADC_USE						ADC1
#define ADC_CLK         	RCC_APB2Periph_ADC1

#define ADC_CHANNEL_NUM		4	
/*******************************************************/

#define ADC_BUFFSIZE	20* ADC_CHANNEL_NUM    //20 ƽ���˲� ����

extern __IO uint16_t ADC_Value[ADC_BUFFSIZE];

 void bsp_ADC_Init(void);							//��ʼ��
#endif
