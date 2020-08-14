#ifndef 	_BSP_RF_H_
#define 	_BSP_RF_H_


/**
  *
  *		
  *
**/



#include "stm32f4xx.h"
#include "BSP_UART.h"
#include "bsp_delay.h"

//引脚定义
/*******************************************************/
#define RF_M0_PIN                  GPIO_Pin_4                 
#define RF_M0_GPIO_PORT            GPIOA                      
#define RF_M0_GPIO_CLK             RCC_AHB1Periph_GPIOA

#define RF_M1_PIN                  GPIO_Pin_5                
#define RF_M1_GPIO_PORT            GPIOB                      
#define RF_M1_GPIO_CLK             RCC_AHB1Periph_GPIOB

#define RF_AUX_PIN                  GPIO_Pin_9                 
#define RF_AUX_GPIO_PORT            GPIOB                      
#define RF_AUX_GPIO_CLK             RCC_AHB1Periph_GPIOB

#define RF_AUX_READ					GPIO_ReadInputDataBit(RF_AUX_GPIO_PORT,RF_AUX_PIN)

#define RF_MOUDLE_SLEEP				GPIO_SetBits(RF_M1_GPIO_PORT,RF_M1_PIN);GPIO_SetBits(RF_M0_GPIO_PORT,RF_M0_PIN);

#define RF_MOUDLE_LOWPWR			GPIO_SetBits(RF_M1_GPIO_PORT,RF_M1_PIN);GPIO_ResetBits(RF_M0_GPIO_PORT,RF_M0_PIN);

#define RF_MOUDLE_WAKEUP			GPIO_ResetBits(RF_M1_GPIO_PORT,RF_M1_PIN);GPIO_SetBits(RF_M0_GPIO_PORT,RF_M0_PIN);

#define RF_MOUDLE_STANBY			GPIO_ResetBits(RF_M1_GPIO_PORT,RF_M1_PIN);GPIO_ResetBits(RF_M0_GPIO_PORT,RF_M0_PIN);



#define RF_UART                             USART1
#define RF_UART_CLK                         RCC_APB2Periph_USART1

#define RF_UART_BAUDRATE                    9600	

#define RF_UART_RX_GPIO_PORT                GPIOA
#define RF_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RF_UART_RX_PIN                      GPIO_Pin_9
#define RF_UART_RX_AF                       GPIO_AF_USART1
#define RF_UART_RX_SOURCE                   GPIO_PinSource9

#define RF_UART_TX_GPIO_PORT                GPIOA
#define RF_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RF_UART_TX_PIN                      GPIO_Pin_10
#define RF_UART_TX_AF                       GPIO_AF_USART1
#define RF_UART_TX_SOURCE                   GPIO_PinSource10

#define RF_UART_IRQHandler                  USART1_IRQHandler
#define RF_UART_IRQ                 	    USART1_IRQn

#define E32_CFG_DEFAULT   {0xC0,0x00,0x00,DATA_RATE_9600,UART_BPS_9600,UART_CHECK_8N1,0x17,0x00,\
							TX_PWR_30DBM,FEC_ENABLE,0x00,IO_OUT_PP_IN_UP,SEND_MODE_TRANS}

#define DATA_RATE_300  		0x00
#define DATA_RATE_1200  	0x01
#define DATA_RATE_2400  	0x02
#define DATA_RATE_4800  	0x03
#define DATA_RATE_9600  	0x04
#define DATA_RATE_19200  	0x05  //0x06 0x07

#define UART_BPS_1200		0x00
#define UART_BPS_2400		0x01
#define UART_BPS_4800		0x02
#define UART_BPS_9600		0x03
#define UART_BPS_19200		0x04
#define UART_BPS_38400		0x05
#define UART_BPS_57600		0x06
#define UART_BPS_115200		0x07

#define UART_CHECK_8N1		0x00		//0x11
#define UART_CHECK_801		0x00
#define UART_CHECK_8E1		0x00
	
#define TX_PWR_30DBM		0x00
#define TX_PWR_27DBM		0x00
#define TX_PWR_24DBM		0x00
#define TX_PWR_21DBM		0x00

#define FEC_DISABLE			0x00
#define FEC_ENABLE			0x01

#define IO_OUT_OD_IN_FLAOT	0x00
#define IO_OUT_PP_IN_UP		0x01

#define SEND_MODE_TRANS		0x00
#define SEND_MODE_FIXED		0x01

typedef struct{
	u8 Head;				//C0:掉电保存，C2掉电不保存
	
	u8 ADDH;				//地址高位
	
	u8 ADDL;				//地址低位
	
	u8 Data_Rate:3;			//空中速率  //默认0x02--2.4K  
	u8 UART_Bps:3;			//串口速率 	//默认0x03--9600
	u8 UART_Check:2;		//串口校验位//默认0x00--8N1
	
	u8 RF_Channel:5;		//通信信道	//410Mhz+Chan+1Mhz		//默认 0x17 433Mhz
	u8 Reserve:3;			//保留位
	
	u8 TX_PWR:2;			//发射功率 默认0x00--30dbm
	u8 FEC:1;				//fec纠错  默认0x01--开启
	u8 Wakeup_Delay:3;		//无限唤醒时间 默认0x00--250ms		
							//Wakeup_Delay = TIME/250 - 1;TIME_MAX 2000
	u8 IO_Mode:1;			//IO驱动方式	默认0x01推挽输出 上拉输入
	u8 Send_Mode:1;			//发送模式 默认0x010--透明传出
}RF_E32_Cfg_Typedef;

typedef enum {RF_TRANS_MODE,RF_CMD_MODE}RF_MODE_TypeDef;

/**************************************************************************/
//RF射频专用协议 BA 9F  Datalen Cmd nData Sum 

/**************************************************************************/
#define RF_DATA_MAX 		128
typedef struct __PACKED{
	u8 Datalen;
	u8 Cmd;
	u8 data[0];
}RF_UART_Buf_Typedef; 

extern RX_Buf_Typedef RF_RX_Buf;

u8 bsp_RF_Send_Config_Cmd(u8 ADDH,u8 ADDL,u8 Channel);
void  bsp_RF_Init(void);
u16 bsp_RF_UART_Parse(void);
u8 bsp_RF_Send_Cmd(u8 Cmd,u32 Datalength,u8*data);

#endif

