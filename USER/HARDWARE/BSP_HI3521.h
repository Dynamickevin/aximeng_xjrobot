#ifndef  	_BSP_HI3521_NTC_H_
#define 	_BSP_HI3521_NTC_H_


#include "stm32f4xx.h"
#include  "BSP_ADC_Init.h"
#include "BSP_UART.h"
#include "string.h"
/*****************************************************/

//*************************/��
//������Ϣ
#define HI3521_NTC_ERROR		0x10




/******************************************/


/*****************************************************/

//���Ŷ���
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



/***********************************����ͨ��********************************/
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


void bsp_HI3521_Heater(void);   //��ȡHI3521 �����¶ȣ�������趨ֵ ��ʼ���ȣ������趨ֵֹͣ����

void bsp_HI3521_GPIO_Init(void);							//��ʼ��
int8_t bsp_HI3521_Ntc_Get_Temp(void);							//��ȡ�¶�
void HI3521_Reset(void);

void bsp_HI3521_Pluse(void);   //�������� 				//������ whileѭ������
u16 bsp_HI3521_UART_Parse(void);
#endif
/**************************************************END LINE**********************************************/

