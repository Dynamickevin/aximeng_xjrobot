

#include <includes.h>

/*********************************debug��ӡ����*****************************/
/**/
/**/
/**/
/************************************************************************/


OS_EVENT	*Uart4Sem;					//�����ӡ�����Ϣ�ź���
AtCmdInfo AtCmdFromDebug;			//����UART����ָ����Ϣ
ZT_INFO_TYPE g_zt_msg4;


/* this is for DEBUG board module */
void bsp_Debug_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  //ʹ��GPIOCʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	//ʹ��UART4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
	//����4��Ӧ���Ÿ���ӳ��	
	/* ���� PC10 �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);

	/*  ���� PC11x �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
 /* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

 
  /* ���ô���Ӳ������ */
	//UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART_InitStructure); //��ʼ������4

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART4, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */

  
  /* Enable UART4 */
	AtCmdFromDebug.counter = 0;
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	
  Uart4Sem = OSSemCreate(1);
  USART_Cmd(UART4, ENABLE);		/* ʹ�ܴ���4 */ 
	 //LED2(LED_ON);

}



/****************************************************************************
* ������: UART4_ISR   UART4 = DEBUG_PORT
* ��  ��: UART4�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void UART4_ISR(void)
{
	uint8 err,err1;
	DATA_CONTROL_BLOCK  *dp;
	uint8 *dp1;
    //char* str;
	
	/* USART in mode Tramitter -------------------------------------------------*/
  if (USART_GetITStatus(UART4, USART_IT_TXE) == SET)
  { /* When Joystick Pressed send the command then send the data */
  		
	  g_zt_msg4.counter = g_zt_msg4.counter+1;
	  
      if (g_zt_msg4.counter >= g_zt_msg4.icmd_len)
      {			
       
				/* Disable the USARTx transmit data register empty interrupt */
          USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
          g_zt_msg4.counter = 0;
					OSSemPost(Uart4Sem);
      }	
	  else
	  {
	  	USART_SendData(UART4, g_zt_msg4.sendbuf[g_zt_msg4.counter]);
	  }
  }

  if (USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
	{	
		err = USART_ReceiveData(UART4);//�ն����ж�

		dp = OSMemGet(p_msgBuffer,&err);
		dp1 = OSMemGet(mem160ptr,&err1);
		if((err == OS_ERR_NONE)&&(err1 == OS_ERR_NONE))
		{
		   dp->type = LOCAL_DEBUG_IN_MSG_DEBUG;
		   dp->count = AtCmdFromDebug.counter;
		   CopyBuffer(&AtCmdFromDebug.recv_buf[0],dp1,AtCmdFromDebug.counter);
		   dp->point = (uint8 *)dp1; 
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
		AtCmdFromDebug.counter = 0;
	}
	
  /* USART4 in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
    {
	    AtCmdFromDebug.recv_buf[AtCmdFromDebug.counter++] = USART_ReceiveData(UART4);
	    AtCmdFromDebug.counter %= RX_LENGTH; 			
    }   
}

//��ӡ�������
void uart4_send(uint8 *sp, uint16 len)
{
    uint8 err; 

    OSSemPend(Uart4Sem,0,&err);
	
	if(OS_ERR_NONE != err)
	{
		return;
	}
    //OSTimeDly(OS_TICKS_PER_SEC/5);
	
	CopyBuffer(sp,&g_zt_msg4.sendbuf[0],len);

	g_zt_msg4.icmd_len = len;
	g_zt_msg4.counter = 0;
	USART_SendData(UART4, g_zt_msg4.sendbuf[0]);
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
}


