#ifndef  	_BSP_HI3521_NTC_H_
#define 	_BSP_HI3521_NTC_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"
#include "BSP_UART.h"
#include "string.h"
/*****************************************************/

//*************************/、
//错误信息
#define HI3521_NTC_ERROR		0x10




/******************************************/


/*****************************************************/

//引脚定义
/*******************************************************/
#define HI3521_RST_GPIO_PORT		GPIOA
#define HI3521_RST_GPIO_PIN			GPIO_Pin_11
#define HI3521_RST_GPIO_CLK			RCC_AHB1Periph_GPIOA

#define HI3521_HEATER_GPIO_PORT		GPIOA
#define HI3521_HEATER_GPIO_PIN			GPIO_Pin_11
#define HI3521_HEATER_GPIO_CLK			RCC_AHB1Periph_GPIOA

#define HI3521_NTC_GPIO_PORT		GPIOC
#define HI3521_NTC_GPIO_PIN			GPIO_Pin_3
#define HI3521_NTC_GPIO_CLK			RCC_AHB1Periph_GPIOC

#define	HI3521_NTC_ADC_CHANNEL    	ADC_Channel_13	

#define HI3521_NTC_ADC_INDEX		3

#define	HI3521_NTC_RES					100000				//100K
#define HI3521_LIMIT_TEMP_MIN				-5
#define HI3521_LIMIT_TEMP_MAX				0



/***********************************串口通信********************************/
#define  HI3521_UART                             USART3
#define  HI3521_UART_CLK                         RCC_APB1Periph_USART3

#define  HI3521_UART_BAUDRATE                    9600	

#define  HI3521_UART_RX_GPIO_PORT                GPIOB
#define  HI3521_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define  HI3521_UART_RX_PIN                      GPIO_Pin_10
#define  HI3521_UART_RX_AF                       GPIO_AF_USART3
#define  HI3521_UART_RX_SOURCE                   GPIO_PinSource10

#define  HI3521_UART_TX_GPIO_PORT                GPIOB
#define  HI3521_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define  HI3521_UART_TX_PIN                      GPIO_Pin_11
#define  HI3521_UART_TX_AF                       GPIO_AF_USART3
#define  HI3521_UART_TX_SOURCE                   GPIO_PinSource11

#define  HI3521_UART_IRQHandler                  USART3_IRQHandler
#define  HI3521_UART_IRQ                 		 USART3_IRQn
/********************************************************************************/


void bsp_HI3521_Heater(void);   //获取HI3521 表面温度，如低于设定值 则开始加热，高于设定值停止加热

void bsp_HI3521_GPIO_Init(void);							//初始化
int8_t bsp_HI3521_Ntc_Get_Temp(void);							//获取温度
void HI3521_Reset(void);

void bsp_HI3521_Pluse(void);   //心跳函数 				//主函数 while循环调用
u16 bsp_HI3521_UART_Parse(void);
#endif
/**************************************************END LINE**********************************************/

