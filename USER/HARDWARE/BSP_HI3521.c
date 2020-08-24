#include  "BSP_HI3521.h"

/**
  ******************************************************************************
  * @file    BSP_HI3521.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   HI3521控制
  ******************************************************************************
  * @attention
  *		
  * 	
  *		
  *		
  *		
  *
  ******************************************************************************
  */

const float NTC_B3950_TAB[130]={
		340.9281,318.8772,298.3978,279.3683,261.6769,245.2212,229.9072,215.6488,202.3666,189.9878,	//-40~-31
		178.4456,167.6783,157.6292,148.246,139.4807,131.2888,123.6294,116.4648,109.76,103.4829,		//-30~-21
		97.6037,92.0947,86.9305,82.0877,77.5442,73.2798,69.2759,65.5149,61.9809,58.6587,			//-20~-11
		55.5345,52.5954,49.8294,47.2253,44.7727,42.462,40.2841,38.2307,36.294,34.4668,				//-10~-1
		32.7421,31.1138,29.5759,28.1229,26.7496,25.4513,24.2234,23.0618,21.9625,20.9218,			//0~9
		19.9364,19.0029,18.1184,17.28,16.4852,15.7313,15.0161,14.3375,13.6932,13.0815,				//10~19
		12.5005,11.9485,11.4239,10.9252,10.451,10,9.5709,9.1626,8.7738,8.4037,						//20~29
		8.0512,7.7154,7.3954,7.0904,6.7996,6.5223,6.2577,6.0053,5.7645,5.5345,						//30~39
		5.315,5.1053,4.905,4.7136,4.5037,4.3558,4.1887,4.0287,3.8758,3.7294,						//40~49
		3.5893,3.4553,3.3269,3.2039,3.0862,2.9733,2.8652,2.7616,2.6622,2.5669,						//50~59
		2.4755,2.3879,2.3038,2.2231,2.1456,2.0712,1.9998,1.9312,1.8653,1.8019,						//60~69
		1.7411,1.6826,1.6264,1.5723,1.5203,1.4703,1.4222,1.3759,1.3313,1.2884,						//70~79
		1.2471,1.2073,1.169,1.1321,1.0965,1.0623,1.0293,0.9974,0.9667,0.9372,						//80~89
//		0.9086,0.8811,0.8545,0.8289,0.8042,0.7803,0.7572,0.735,0.7135,0.6927,						//90~99
//		0.6727,																						//100
};
extern u8    RobotError;						//错误
int8_t Ntc_temp;

Pluse_Info_Typedef HI3521_Pluse_Info;
RX_Buf_Typedef HI3521_RX_Buf;
extern uint32_t Time100ms;



static void  bsp_HI3521_UART_Init(void);
 /*
  * @brief  HI3521 IO初始化
  * @param  无
  * @retval 无
*/
void bsp_HI3521_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能 GPIO 时钟
	
	RCC_AHB1PeriphClockCmd(HI3521_NTC_GPIO_CLK|HI3521_RST_GPIO_CLK|HI3521_HEATER_GPIO_CLK, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = HI3521_RST_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //低电平使能
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed ;
	GPIO_Init(HI3521_RST_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HI3521_RST_GPIO_PORT,HI3521_RST_GPIO_PIN);
	
	GPIO_InitStructure.GPIO_Pin = HI3521_HEATER_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ; //高电平加热
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed ;
	GPIO_Init(HI3521_HEATER_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HI3521_HEATER_GPIO_PORT,HI3521_HEATER_GPIO_PIN);	
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = HI3521_NTC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(HI3521_NTC_GPIO_PORT, &GPIO_InitStructure);	
		
	bsp_HI3521_UART_Init();
}

 /*
  * @brief  HI3521_UART初始化函数
  * @param  无
  * @retval 无
*/
static void  bsp_HI3521_UART_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(  HI3521_UART_RX_GPIO_CLK| HI3521_UART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART 时钟 */
	RCC_APB1PeriphClockCmd( HI3521_UART_CLK, ENABLE);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig( HI3521_UART_RX_GPIO_PORT, HI3521_UART_RX_SOURCE,  HI3521_UART_RX_AF);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig( HI3521_UART_TX_GPIO_PORT, HI3521_UART_TX_SOURCE, HI3521_UART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin =  HI3521_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( HI3521_UART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =  HI3521_UART_RX_PIN;
	GPIO_Init( HI3521_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置串口 HI3521_UART 模式 */
	USART_InitStructure.USART_BaudRate =  HI3521_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( HI3521_UART, &USART_InitStructure); 


	/*配置串口接收中断*/
	USART_ITConfig( HI3521_UART, USART_IT_RXNE, ENABLE);

	USART_Cmd( HI3521_UART, ENABLE);
}







/**************************************************END LINE**********************************************/
