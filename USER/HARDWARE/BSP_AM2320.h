#ifndef  	_BSP_AM2320_H_
#define 	_BSP_AM2320_H_


#include "stm32f4xx.h"
#include  "bsp_delay.h"


//引脚定义
/*******************************************************/

#define AM2320_IO_SDA_PIN                  GPIO_Pin_9                 
#define AM2320_IO_SDA_GPIO_PORT            GPIOC                  
#define AM2320_IO_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define AM2320_DATA_OUT(a)	if (a)	\
                                   GPIO_SetBits(AM2320_IO_SDA_GPIO_PORT,AM2320_IO_SDA_PIN);\
                                   else		\
                                   GPIO_ResetBits(AM2320_IO_SDA_GPIO_PORT,AM2320_IO_SDA_PIN)

#define  AM2320_DATA_IN()	  GPIO_ReadInputDataBit(AM2320_IO_SDA_GPIO_PORT,AM2320_IO_SDA_PIN)


#define AM2320_HIGH  1
#define AM2320_LOW   0

typedef struct
{
	float  humi;//_High;		//湿度
	float  temp;//_Low;	 	//温度
}AM2320_Data_TypeDef;


void bsp_AM2320_Init(void);
uint8_t bsp_AM2320_Read_Datas(AM2320_Data_TypeDef *DHT11_Data);

#endif
