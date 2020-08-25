/****************************************************************************
	ע�⣺
	(1) �жϳ���δ�Է��ͻ�������������.Ҳ����˵����comSendBuf()����ʱ�����
		���ٵķ��ͻ��������㣬���������ȷ���FIFO�����ݱ������
		��������һ���Ӧ�ó��򣬽����ͻ����������㹻���Ա�֤һ�����ͨ��֡�ܹ������¼��ɡ�*/
/* Includes ------------------------------------------------------------------*/
#include <includes.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include "uart_api.h"

/* ���崮�ڽṹ����� */
static UART_T g_tUart1;

static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);

OS_EVENT	*Usart1Sem;
AtCmdInfo AtCmdFromRF433;
ZT_INFO_TYPE g_zt_msg1;

OS_EVENT	*Usart2Sem;
AtCmdInfo AtCmdFromPowerBoard;
ZT_INFO_TYPE g_zt_msg2;

OS_EVENT	*Usart3Sem;
AtCmdInfo AtCmdFromLinux;
ZT_INFO_TYPE g_zt_msg3;



OS_EVENT	*Uart4Sem;
AtCmdInfo AtCmdFromDebug;
ZT_INFO_TYPE g_zt_msg4;

OS_EVENT	*Uart5Sem;
AtCmdInfo AtCmdFromPTZ;
ZT_INFO_TYPE g_zt_msg5;


//CmdParaInfo gCmdParaInfos;//,gCmdParaInfos_RF433,gCmdParaInfos_DEBUG,gCmdParaInfos_LINUX;
extern RbtState gRbtState;

/****************************************************************************
* ������: comSendBuf
* ��  ��: �򴮿ڷ���һ�����ݣ���������
* ��  ��: 	_ucPort: �˿ں�(COM1)
*			_ucaBuf: �����͵����ݻ�����
*			_usLen : ���ݳ���
* ��  ��: ��
* ��  ��: ��
*/
void comSendBuf(uint8_t _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;
		
	if (_ucPort == COM1)
	{
		pUart = &g_tUart1;
		
	}
	else
	{
		/* �����κδ��� */
		return;
	}

	UartSend(pUart, _ucaBuf, _usLen);	
}

/****************************************************************************
* ������: comSendChar
* ��  ��: �򴮿ڷ���1���ֽڣ���������
* ��  ��: 	_ucPort: �˿ں�(COM1 COM2 COM3)
*			_ucByte: �����͵�����
* ��  ��: ��
* ��  ��: ��
*/
void comSendChar(uint8_t _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}




/* this is for DEBUG module */
void USART1_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  //ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	//ʹ��USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  
	//����1��Ӧ���Ÿ���ӳ��	
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);

	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
 /* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 
  /* ���ô���Ӳ������ */
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */
 
  /* Enable USART1 */
	AtCmdFromRF433.counter = 0;
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	//LED2(LED_ON);
	
  Usart1Sem = OSSemCreate(1);

	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */ 

}

/* this is for POWER BOARD module */
void USART2_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	//ʹ��USART2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  
	//����2��Ӧ���Ÿ���ӳ��	
	/* ���� PA2 �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	/*  ���� PA3 �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
 /* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 
  /* ���ô���Ӳ������ */
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */

  /* Enable USART */
	AtCmdFromPowerBoard.counter = 0;
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
  Usart2Sem = OSSemCreate(1);
  USART_Cmd(USART2, ENABLE);		/* ʹ�ܴ���2 */ 

}


/* this is for ��˼оƬ LINUX module */
void USART3_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  //ʹ��GPIOBʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	//ʹ��USART3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
	//����3��Ӧ���Ÿ���ӳ��	
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2);

	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2);
	
 /* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 
  /* ���ô���Ӳ������ */
	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */

  
  /* Enable USART */
	AtCmdFromLinux.counter = 0;
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
  Usart3Sem = OSSemCreate(1);
  USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ���3 */ 
	 //LED2(LED_ON);

}
/* this is for POWER board module */
void UART4_Config(void)
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
* ������: UartSend
* ��  ��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ�
*			�رշ����жϡ�
* ��  ��: _pUart: ���ڽṹ��ָ��
* ��  ��: ��
* ��  ��: ��
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucNew = 0;
    
	CPU_IntDis();
	if (_pUart->usTxWrite == _pUart->usTxRead)
	{
		ucNew = 1;
	}
	CPU_IntEn();
	
	for (i = 0; i < _usLen; i++)
	{	
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];
		
		CPU_IntDis();	
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		CPU_IntEn();
	}

	if (ucNew == 1)
	{
		USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
	}
}



/****************************************************************************
* ������: USART1_ISR   USART1 = LORA_PORT
* ��  ��: USART1�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
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
		err = USART_ReceiveData(USART1);//�ն����ж�
		
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

/****************************************************************************
* ������: USART2_ISR   USART2 = POWER_BOARD_PORT
* ��  ��: USART2�жϷ������.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
void USART2_ISR(void)
{
	uint8 err,err1;
	DATA_CONTROL_BLOCK  *dp;
	uint8 *dp1;
    //char* str;
	
	/* USART in mode Tramitter -------------------------------------------------*/
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  { /* When Joystick Pressed send the command then send the data */
  		
	  g_zt_msg2.counter = g_zt_msg2.counter+1;
	  
      if (g_zt_msg2.counter >= g_zt_msg2.icmd_len)
      {			
          //GpioSetL(GPIO_LED_SHOW1);
		  /* Disable the USARTx transmit data register empty interrupt */
          USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
          g_zt_msg2.counter = 0;
					OSSemPost(Usart2Sem);
      }	
	  else
	  {
	  	USART_SendData(USART2, g_zt_msg2.sendbuf[g_zt_msg2.counter]);
	  }
  }

  if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{	
		err = USART_ReceiveData(USART2);//�ն����ж�

		dp = OSMemGet(p_msgBuffer,&err);
		dp1 = OSMemGet(mem160ptr,&err1);
		if((err == OS_ERR_NONE)&&(err1 == OS_ERR_NONE))
		{
		   dp->type = LOCAL_DEBUG_IN_MSG_POWER_BOARD;
		   dp->count = AtCmdFromPowerBoard.counter;
		   CopyBuffer(&AtCmdFromPowerBoard.recv_buf[0],dp1,AtCmdFromPowerBoard.counter);
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
		AtCmdFromPowerBoard.counter = 0;
	}
	
  /* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
	    AtCmdFromPowerBoard.recv_buf[AtCmdFromPowerBoard.counter++] = USART_ReceiveData(USART2);
	    AtCmdFromPowerBoard.counter %= RX_LENGTH; 			
    }   
}


/****************************************************************************
* ������: USART3_ISR   USART3 = LINUX_PORT
* ��  ��: USART3�жϷ������
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
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
		err = USART_ReceiveData(USART3);//�ն����ж�

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


//LORA��Ϣ�������
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

//��Դ��ͨ�Ŵ���
void uart2_send(uint8 *sp, uint16 len)
{
    uint8 err; 

    OSSemPend(Usart2Sem,0,&err);
	
	if(OS_ERR_NONE != err)
	{
		return;
	}
    //OSTimeDly(OS_TICKS_PER_SEC/5);
	
	CopyBuffer(sp,&g_zt_msg2.sendbuf[0],len);

	g_zt_msg2.icmd_len = len;
	g_zt_msg2.counter = 0;
	USART_SendData(USART2, g_zt_msg2.sendbuf[0]);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

//��˼оƬlinuxͨ�Ŵ���
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


