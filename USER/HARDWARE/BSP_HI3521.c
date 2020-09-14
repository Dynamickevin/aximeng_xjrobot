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


OS_EVENT	*Usart3Sem;
AtCmdInfo AtCmdFromLinux;
ZT_INFO_TYPE g_zt_msg3;

u8    RobotError;						//错误		

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



/* this is for 海思芯片 LINUX module */
void bsp_HI3521_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  //使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	//使能USART3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
	//串口3对应引脚复用映射	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2);
	
 /* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 
  /* 设置串口硬件参数 */
	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口3

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */

  
  /* Enable USART */
	AtCmdFromLinux.counter = 0;
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
  Usart3Sem = OSSemCreate(1);
  USART_Cmd(USART3, ENABLE);		/* 使能串口3 */ 
	 //LED2(LED_ON);

}

/****************************************************************************
* 函数名: USART3_ISR   USART3 = LINUX_PORT
* 功  能: USART3中断服务程序
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void USART3_ISR(void)
{
	uint8 err,err1;
	DATA_CONTROL_BLOCK  *dp;
	uint8 *dp1;
   // char* str;
	
	/* USART in mode Tramitter -------------------------------------------------*/
  if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
  { /* When Joystick Pressed send the command then send the data */
  		
		g_zt_msg3.counter = g_zt_msg3.counter+1;
	  
      if (g_zt_msg3.counter >= g_zt_msg3.icmd_len)
      {				
          /* Disable the USARTx transmit data register empty interrupt */
          USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
          g_zt_msg3.counter = 0;
					OSSemPost(Usart3Sem);
      }	
	  else
	  {
	  	USART_SendData(USART3, g_zt_msg3.sendbuf[g_zt_msg3.counter]);
	  }
  }

  if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{	
		err = USART_ReceiveData(USART3);//空读清中断

		dp = OSMemGet(p_msgBuffer,&err);
		dp1 = OSMemGet(mem160ptr,&err1);
		if((err == OS_ERR_NONE)&&(err1 == OS_ERR_NONE))
		{  
       //GpioSetL(GPIO_LED_SHOW1);
		   dp->type = LOCAL_DEBUG_IN_MSG_LINUX;
		   dp->count = AtCmdFromLinux.counter;
		   CopyBuffer(&AtCmdFromLinux.recv_buf[0],dp1,AtCmdFromLinux.counter);
		   dp->point = (uint8 *)(dp1); 
		   	if(OS_ERR_NONE != OSQPost(RemoteRecQueue,(void*)dp))
		    {
			    OSMemPut(mem160ptr,dp1);
			    OSMemPut(p_msgBuffer,dp);
		    }	
		   	
		}
		else
		{
			if(err== OS_ERR_NONE)
			{
				OSMemPut(p_msgBuffer,dp);
			}	
			if(err1== OS_ERR_NONE)
			{
				OSMemPut(mem160ptr,dp1);
			}
		}
		AtCmdFromLinux.counter = 0;
	}
	
  /* USART in mode Receiver --------------------------------------------------*/
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  {	  
	  AtCmdFromLinux.recv_buf[AtCmdFromLinux.counter++] = USART_ReceiveData(USART3);
	  AtCmdFromLinux.counter %= RX_LENGTH; 			
  }   
}

//海思芯片linux通信串口
void uart3_send(uint8 *sp, uint16 len)
{
    uint8 err; 

    OSSemPend(Usart3Sem,0,&err);
	if(OS_ERR_NONE != err)
	{
		return;
	}

	CopyBuffer(sp,&g_zt_msg3.sendbuf[0],len);

	g_zt_msg3.icmd_len = len;
	g_zt_msg3.counter = 0;
	USART_SendData(USART3, g_zt_msg3.sendbuf[0]);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	
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
	for(i = 0 ; i < ADC_BUFFSIZE/ADC_CHANNEL_NUM ; i++)
	{
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
