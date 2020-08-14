#ifndef 	_BSP_BRD_H_
#define 	_BSP_BRD_H_


/**
  *
  *		
  *
**/



#include "stm32f4xx.h"
#include "BSP_UART.h"
#include "bsp_delay.h"
#include  "board.h"
//Òý½Å¶¨Òå
/*******************************************************/

#define BRD_UART                             USART2
#define BRD_UART_CLK                         RCC_APB1Periph_USART2

#define BRD_UART_BAUDRATE                    9600	

#define BRD_UART_RX_GPIO_PORT                GPIOA
#define BRD_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_RX_PIN                      GPIO_Pin_2
#define BRD_UART_RX_AF                       GPIO_AF_USART2
#define BRD_UART_RX_SOURCE                   GPIO_PinSource2

#define BRD_UART_TX_GPIO_PORT                GPIOA
#define BRD_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_TX_PIN                      GPIO_Pin_3
#define BRD_UART_TX_AF                       GPIO_AF_USART2
#define BRD_UART_TX_SOURCE                   GPIO_PinSource3

#define BRD_UART_IRQHandler                  USART2_IRQHandler
#define BRD_UART_IRQ                 		 USART2_IRQn


typedef struct __PACKED {
	u16			Total_voltage;
	u16			Electric_current;
	int16_t 	BTemp[2];
	u16 		Monomer_voltage[6];
	u8			Sw_Status;
} BRD_Info_Typedef;



extern BRD_Info_Typedef BRD_Info; 
extern RX_Buf_Typedef BRD_RX_Buf;
extern Pluse_Info_Typedef BRD_Pluse_Info;

void  bsp_BRD_Init(void);
void bsp_BRD_Pluse(void);
u16 bsp_BRD_UART_Parse(void);
#endif

