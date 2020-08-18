
#ifndef 	_WHEEL_ENCODE_H_
#define 	_WHEEL_ENCODE_H_


#include "stm32f4xx.h"


void  wheel_encode_Init(void);


void  wheel_encode_GPIO_Init(void );

void EXTI15_10_IRQ_ISR(void);

#endif


