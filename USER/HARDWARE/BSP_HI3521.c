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

/*
  * @brief  HI3521 串口处理函数
  * @param  无
  * @retval 无
*/
void HI3521_UART_IRQHandler(void)
{
	u8 tmp;
	if(USART_GetITStatus(HI3521_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(HI3521_UART);
		if(HI3521_RX_Buf.Status == UART_END)return;
		else if(HI3521_RX_Buf.Status == UART_READY){
			HI3521_RX_Buf.Index = 0;
		}
		HI3521_RX_Buf.Timeout = RX_TIMEOUT;
		HI3521_RX_Buf.data[HI3521_RX_Buf.Index] =tmp;
		switch(HI3521_RX_Buf.Status){
			case 0:
				if(HI3521_RX_Buf.data[HI3521_RX_Buf.Index] == UART_HEAD1)
					HI3521_RX_Buf.Status = 1;
				break;
			case 1:
				if(HI3521_RX_Buf.data[HI3521_RX_Buf.Index] == UART_HEAD2)
					HI3521_RX_Buf.Status = 2;
				else HI3521_RX_Buf.Status = 0;
				break;
			case 2:
				if(HI3521_RX_Buf.Index == 7){
					HI3521_RX_Buf.Datalen = *(u16*)(HI3521_RX_Buf.data+4);
					if(HI3521_RX_Buf.Datalen < DATA_MAX + 16)
						HI3521_RX_Buf.Status = 3;
					else HI3521_RX_Buf.Status = 0;
				}
				break;
			case  3:
				if(HI3521_RX_Buf.Index >= HI3521_RX_Buf.Datalen - 1){
					HI3521_Pluse_Info.Last_Time = Time100ms;
					HI3521_RX_Buf.Status = UART_END;
					HI3521_Pluse_Info.Status = 0;
					HI3521_Pluse_Info.Expired_times = 0;
				}
				break;
			default:
				break;
		}		
		HI3521_RX_Buf.Index++;		
	}
}



/*
* @brief  HI3521 主动发送一条指令（需要返回包）
  * @param  无
  * @retval 0 成功  1 失败
*/
u8 bsp_HI3521_Send_Cmd(u8 Cmd,u32 Datalength,u32 param,u8*data)
{
	u8 Cnt;
	UART_Send_Cmd(HI3521_UART,STM32_HI3521_CMD1|STM_TX_GET_STAUS,Datalength,param,data);
	delay_ms(10);
	Cnt = 20;
	while(1){
		if(HI3521_RX_Buf.Status == UART_END)
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
* @brief  HI3521 串口发送指令（带3次重发）
  * @param  无
  * @retval 0
*/
void bsp_HI3521_Pluse(void)
{
	if(Time100ms - HI3521_Pluse_Info.Last_Time > 100){
		if(HI3521_Pluse_Info.Status != 0 )HI3521_Pluse_Info.Expired_times ++;
		UART_Send_Cmd(HI3521_UART,STM32_HI3521_CMD1|STM_TX_GET_STAUS,0,0,0);
		HI3521_Pluse_Info.Status = 1;
		HI3521_Pluse_Info.Last_Time = Time100ms;
	}
}


/*
* @brief  HI3521 串口接收数据解析函数
  * @param  无
  * @retval 收到的指令
*/
u16 bsp_HI3521_UART_Parse(void)
{
	u32 CRC32_Value;
	if(HI3521_RX_Buf.Status == UART_END){			
		memcpy(&UART_Rx_Buf,HI3521_RX_Buf.data+2,HI3521_RX_Buf.Datalen-2);
		HI3521_RX_Buf.Status = UART_READY;
		CRC32_Value = UART_Rx_Buf.Crc32;
		UART_Rx_Buf.Crc32 = 0 ;
		UART_Rx_Buf.Crc32 = crc32(0,(void *)&UART_Rx_Buf,UART_Rx_Buf.Packet_Length -2);
		if(CRC32_Value == UART_Rx_Buf.Crc32){
			if(UART_Rx_Buf.Cmd1 == 0x31){
				//处理接收数据
				switch(UART_Rx_Buf.Cmd2){
					case STM_RX_REPLY_STAUS:
					//	memcpy(&HI3521_Info,UART_Rx_Buf.Data,sizeof(HI3521_Info));
						break;
					case UART_ACK:
						break;
					case HI3521_TX_GET_STATUS:
						break;
					case HI3521_TX_CTRL_CMD:
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



/*
* @brief  HI3521 硬件复位函数
  * @param  无
  * @retval 无
*/

void HI3521_Reset(void)
{
	GPIO_SetBits(HI3521_RST_GPIO_PORT,HI3521_RST_GPIO_PIN);
	delay_ms(100);
	GPIO_ResetBits(HI3521_RST_GPIO_PORT,HI3521_RST_GPIO_PIN);
}





/*
  * @brief  获取NTC AD采样值
  * @param  无
  * @retval AD值
*/
uint16_t HI3521_Ntc_Get_ADC(void)
{
	u8 i;
	u32 Adc_Sum = 0;
	float tmp;
	for(i = 0 ; i < ADC_BUFFSIZE/ADC_CHANNEL_NUM ; i++){
		Adc_Sum += ADC_Value[HI3521_NTC_ADC_INDEX + ADC_CHANNEL_NUM*i];
	}
	tmp = Adc_Sum/(ADC_BUFFSIZE/ADC_CHANNEL_NUM);
	return tmp;
}



/*
  * @brief  获取当前温度
  * @param  无
  * @retval 返回温度（单位℃）
*/

int8_t bsp_HI3521_Ntc_Get_Temp(void)
{
	static int8_t Temp = 50;
	uint16_t ADC_Value;
	float Res;
	ADC_Value 	= 	HI3521_Ntc_Get_ADC();
	Res 		= 	(float)ADC_Value * HI3521_NTC_RES/4096/1000;
	if(Res < NTC_B3950_TAB[89])return  -100;
	if(Res > NTC_B3950_TAB[Temp])Temp--;
	if(Res < NTC_B3950_TAB[Temp])Temp++;
	return Temp - 40;
}
/*
  * @brief  加热处理函数 低于设定值开始加热  高于设定值停止加热
  * @param  无
  * @retval 无
*/
void bsp_HI3521_Heater(void)
{
	static u8 HeaterFlag = 0;
	Ntc_temp = bsp_HI3521_Ntc_Get_Temp();
	if( Ntc_temp != -100){
		if(Ntc_temp < HI3521_LIMIT_TEMP_MIN && HeaterFlag ==0  ){
			GPIO_SetBits(HI3521_HEATER_GPIO_PORT,HI3521_HEATER_GPIO_PIN);		
			HeaterFlag = 1;
		}else if(Ntc_temp >= HI3521_LIMIT_TEMP_MAX && HeaterFlag){
			HeaterFlag = 0;
			GPIO_ResetBits(HI3521_HEATER_GPIO_PORT,HI3521_HEATER_GPIO_PIN);		
		}
	}else{		
		RobotError = HI3521_NTC_ERROR;
	}
}


/**************************************************END LINE**********************************************/
