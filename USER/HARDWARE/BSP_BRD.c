#include "BSP_BRD.h"
#include <math.h>
#include "string.h"
/************************************************************************/
/*********************************电源板控制*****************************/
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/************************************************************************/

 /*
  * @brief  BRD初始化函数，BRD在使用前必须先调用此函数
  * @param  无
  * @retval 无
*/

BRD_Info_Typedef BRD_Info; 
RX_Buf_Typedef BRD_RX_Buf;
extern uint32_t Time100ms;

Pluse_Info_Typedef BRD_Pluse_Info = {0};
void  bsp_BRD_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd( BRD_UART_RX_GPIO_CLK|BRD_UART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART 时钟 */
	RCC_APB1PeriphClockCmd(BRD_UART_CLK, ENABLE);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(BRD_UART_RX_GPIO_PORT,BRD_UART_RX_SOURCE, BRD_UART_RX_AF);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(BRD_UART_TX_GPIO_PORT,BRD_UART_TX_SOURCE,BRD_UART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = BRD_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRD_UART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = BRD_UART_RX_PIN;
	GPIO_Init(BRD_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置串口BRD_UART 模式 */
	USART_InitStructure.USART_BaudRate = BRD_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BRD_UART, &USART_InitStructure); 


	/*配置串口接收中断*/
	USART_ITConfig(BRD_UART, USART_IT_RXNE, ENABLE);

	USART_Cmd(BRD_UART, ENABLE);
}
/*
  * @brief  BRD 串口处理函数
  * @param  无
  * @retval 无
*/
void BRD_UART_IRQHandler(void)
{
	u8 tmp;
	if(USART_GetITStatus(BRD_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(BRD_UART);
		if(BRD_RX_Buf.Status == UART_END)return;
		else if(BRD_RX_Buf.Status == UART_READY){
			BRD_RX_Buf.Index = 0;
		}
		BRD_RX_Buf.Timeout = RX_TIMEOUT;
		BRD_RX_Buf.data[BRD_RX_Buf.Index] =tmp;
		switch(BRD_RX_Buf.Status){
			case 0:
				if(BRD_RX_Buf.data[BRD_RX_Buf.Index] == UART_HEAD1)
					BRD_RX_Buf.Status = 1;
				break;
			case 1:
				if(BRD_RX_Buf.data[BRD_RX_Buf.Index] == UART_HEAD2)
					BRD_RX_Buf.Status = 2;
				else BRD_RX_Buf.Status = 0;
				break;
			case 2:
				if(BRD_RX_Buf.Index == 7){
					BRD_RX_Buf.Datalen = *(u16*)(BRD_RX_Buf.data+4);
					if(BRD_RX_Buf.Datalen < DATA_MAX + 16)
						BRD_RX_Buf.Status = 3;
					else BRD_RX_Buf.Status = 0;
				}
				break;
			case  3:
				if(BRD_RX_Buf.Index >= BRD_RX_Buf.Datalen - 1){
					BRD_Pluse_Info.Last_Time = Time100ms;
					BRD_RX_Buf.Status = UART_END;
					BRD_Pluse_Info.Status = 0;
					BRD_Pluse_Info.Expired_times = 0;
				}
				break;
			default:
				break;
		}		
		BRD_RX_Buf.Index++;		
	}
}


/*
* @brief  BRD 主动发送一条指令（需要返回包）
  * @param  无
  * @retval 0 成功  1 失败
*/
u8 bsp_BRD_Send_Cmd(u8 Cmd,u32 Datalength,u32 param,u8*data)
{
	u8 Cnt;
	UART_Send_Cmd(BRD_UART,STM32_BRD_CMD1|Cmd,Datalength,param,data);
	delay_ms(10);
	Cnt = 20;
	while(1){
		if(BRD_RX_Buf.Status == UART_END)
			break;	
		delay_ms(10);
		Cnt--;
		if(Cnt == 0){
			return 1;
		}
	}
	return 0;
}


/*
* @brief  BRD 串口发送指令（带3次重发）
  * @param  无
  * @retval 0
*/
void bsp_BRD_Pluse(void)
{
	if(Time100ms - BRD_Pluse_Info.Last_Time > 100){
		if(BRD_Pluse_Info.Status != 0 )BRD_Pluse_Info.Expired_times ++;
		UART_Send_Cmd(BRD_UART,STM32_BRD_CMD1|STM_TX_GET_STAUS,0,0,0);
		BRD_Pluse_Info.Status = 1;
		BRD_Pluse_Info.Last_Time = Time100ms;
	}
}


/*
* @brief  BRD 串口接收数据解析函数
  * @param  无
  * @retval 收到的指令
*/
u16 bsp_BRD_UART_Parse(void)
{
	u32 CRC32_Value;
	if(BRD_RX_Buf.Status == UART_END){			
		memcpy(&UART_Rx_Buf,BRD_RX_Buf.data+2,BRD_RX_Buf.Datalen-2);
		BRD_RX_Buf.Status = UART_READY;
		CRC32_Value = UART_Rx_Buf.Crc32;
		UART_Rx_Buf.Crc32 = 0 ;
		UART_Rx_Buf.Crc32 = crc32(0,(void *)&UART_Rx_Buf,UART_Rx_Buf.Packet_Length -2);
		if(CRC32_Value == UART_Rx_Buf.Crc32){
			if(UART_Rx_Buf.Cmd1 == 0x31){
				//处理接收数据
				switch(UART_Rx_Buf.Cmd2){
					case STM_RX_REPLY_STAUS:
						memcpy(&BRD_Info,UART_Rx_Buf.Data,sizeof(BRD_Info));
						break;
					case UART_ACK:
						break;
					default:
						break;
				}
			}else{
			
			
			}		
		}
	}
	return UART_Rx_Buf.Cmd2;
}
