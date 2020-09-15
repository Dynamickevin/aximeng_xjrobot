

#include <includes.h>

/*********************************LORA通信串口*****************************/
/**/
/**/
/**/
/************************************************************************/

OS_EVENT	*Usart1Sem;
AtCmdInfo AtCmdFromRF433;
ZT_INFO_TYPE g_zt_msg1;

/* this is for LORA module */
void bsp_Lora_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  //使能GPIOA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	//使能USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  
	//串口1对应引脚复用映射	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
 /* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 
  /* 设置串口硬件参数 */
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
 
  /* Enable USART1 */
	AtCmdFromRF433.counter = 0;
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	//LED2(LED_ON);
	
  Usart1Sem = OSSemCreate(1);

	USART_Cmd(USART1, ENABLE);		/* 使能串口 */ 

}



/****************************************************************************
* 函数名: USART1_ISR   USART1 = LORA_PORT
* 功  能: USART1中断服务程序.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
void USART1_ISR(void)
{
	uint8 err,err1;
	DATA_CONTROL_BLOCK  *dp;
	uint8 *dp1;
	//char* str;
	
	/* USART in mode Tramitter -------------------------------------------------*/
  if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
  { /* When Joystick Pressed send the command then send the data */
  		
		g_zt_msg1.counter = g_zt_msg1.counter+1;
	  
      if (g_zt_msg1.counter >= g_zt_msg1.icmd_len)
      {				
          /* Disable the USARTx transmit data register empty interrupt */
          USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
          g_zt_msg1.counter = 0;
					OSSemPost(Usart1Sem);
					//LED2(LED_ON);
      }	
	  else
	  {
	  	USART_SendData(USART1, g_zt_msg1.sendbuf[g_zt_msg1.counter]);
		
	  }
  }

  if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{	
		err = USART_ReceiveData(USART1);//空读清中断
		
		dp = OSMemGet(p_msgBuffer,&err);
		dp1 = OSMemGet(mem160ptr,&err1);
		if((err == OS_ERR_NONE)&&(err1 == OS_ERR_NONE))
		{
		   dp->type = LOCAL_DEBUG_IN_MSG_RF433;
		   dp->count = AtCmdFromRF433.counter;
		   CopyBuffer(&AtCmdFromRF433.recv_buf[0],dp1,AtCmdFromRF433.counter);
		   dp->point = (uint8 *)(dp1); 
		   //str = (char*)(dp->point+3);
		   //cmd_para_get_all_items( (char*)(str) , &gCmdParaInfos);
		  
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
		AtCmdFromRF433.counter = 0;
	}
	
  /* USART in mode Receiver --------------------------------------------------*/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
	  AtCmdFromRF433.recv_buf[AtCmdFromRF433.counter++] = USART_ReceiveData(USART1);
	  AtCmdFromRF433.counter %= RX_LENGTH; 	
  }   
}



//LORA信息输出串口
void uart1_send(uint8 *sp, uint16 len)
{
    uint8 err; 
 
    OSSemPend(Usart1Sem,0,&err);
    //LED2(LED_ON);
	if(OS_ERR_NONE != err)
	{
		return;
	}
	CopyBuffer(sp,&g_zt_msg1.sendbuf[0],len);

	g_zt_msg1.icmd_len = len;
	g_zt_msg1.counter = 0;
	USART_SendData(USART1, g_zt_msg1.sendbuf[0]);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
}








