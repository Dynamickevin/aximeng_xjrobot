#include "BSP_RF.h"
#include  "board.h"
/************************************************************************/
/*********************************RF���߿���*****************************/
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

RX_Buf_Typedef RF_RX_Buf;

RF_MODE_TypeDef RF_MODE = RF_TRANS_MODE;

RF_UART_Buf_Typedef RF_RX_CMD_Buf;

 /*
  * @brief  RF��ʼ��������RF��ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/

void  bsp_RF_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd( RF_UART_RX_GPIO_CLK|RF_UART_TX_GPIO_CLK|RF_AUX_GPIO_CLK|\
							RF_M0_GPIO_CLK|RF_M1_GPIO_CLK, ENABLE);

	/* ʹ�� UART ʱ�� */
	RCC_APB2PeriphClockCmd(RF_UART_CLK, ENABLE); 

	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(RF_UART_RX_GPIO_PORT,RF_UART_RX_SOURCE, RF_UART_RX_AF);

	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(RF_UART_TX_GPIO_PORT,RF_UART_TX_SOURCE,RF_UART_TX_AF);

	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = RF_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RF_UART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = RF_M0_PIN  ;
	GPIO_Init(RF_M0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RF_M1_PIN  ;
	GPIO_Init(RF_M1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = RF_AUX_PIN  ;
	GPIO_Init(RF_AUX_GPIO_PORT, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = RF_UART_RX_PIN;
	GPIO_Init(RF_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* ���ô���RF_UART ģʽ */
	USART_InitStructure.USART_BaudRate = RF_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(RF_UART, &USART_InitStructure); 


	/*���ô��ڽ����ж�*/
	USART_ITConfig(RF_UART, USART_IT_RXNE, ENABLE);

	USART_Cmd(RF_UART, ENABLE);
}
/*
  * @brief  RF ���ڴ�����
  * @param  ��
  * @retval ��
*/
void RF_UART_IRQHandler(void)
{
	u8 tmp;
	static u8 sum;
	if(USART_GetITStatus(RF_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(RF_UART);
		if(RF_RX_Buf.Status == UART_END)return;
		else if(RF_RX_Buf.Status == UART_READY){
			RF_RX_Buf.Index = 0;
		}
		RF_RX_Buf.Timeout = RX_TIMEOUT;
		RF_RX_Buf.data[RF_RX_Buf.Index] =tmp;
		if(RF_MODE == RF_CMD_MODE){
			RF_RX_Buf.Status = 1;
			if(RF_RX_Buf.Index == 5)RF_RX_Buf.Status = UART_END;
		}else {
			switch(RF_RX_Buf.Status){
				case 0:
					if(RF_RX_Buf.data[RF_RX_Buf.Index] == UART_HEAD1)
						RF_RX_Buf.Status = 1;
					break;
				case 1:
					if(RF_RX_Buf.data[RF_RX_Buf.Index] == UART_HEAD2)
						RF_RX_Buf.Status = 2;
					else RF_RX_Buf.Status = 0;
					break;
				case 2:
					if(RF_RX_Buf.data[RF_RX_Buf.Index] < RF_DATA_MAX && RF_RX_Buf.data[RF_RX_Buf.Index]>=5){
						RF_RX_Buf.Datalen = RF_RX_Buf.data[RF_RX_Buf.Index];
						RF_RX_Buf.Status = 3;
						sum= 0;
					}else RF_RX_Buf.Status = 0;
					break;
				case  3:
					if(RF_RX_Buf.Index >= RF_RX_Buf.Datalen - 1){
						if(RF_RX_Buf.data[RF_RX_Buf.Index] == sum)
							RF_RX_Buf.Status = UART_END;
					}else{
						sum +=  RF_RX_Buf.data[RF_RX_Buf.Index];
					}
					break;
				default:
					break;
			}		
		}
		RF_RX_Buf.Index++;		
	}
}


/*
* @brief  RF E32����ָ��
  * @param  ADDH����ַ��λ
  *			ADDL����ַ��λ
  *			Channel��ͨ��Ƶ�� =(Channel+410)Mhz
  * @retval 0 �ɹ�  ���� ʧ��
  *			1 ��������
  *			2 ģ�����
  *			3.����ֵ�����ò����
  *			
*/
u8 bsp_RF_Send_Config_Cmd(u8 ADDH,u8 ADDL,u8 Channel)
{
	u8 i,cnt;
	
	RF_E32_Cfg_Typedef E32_CFG = E32_CFG_DEFAULT;
	E32_CFG.ADDH = ADDH;
	E32_CFG.ADDL = ADDL;
	E32_CFG.RF_Channel = Channel;
	if(E32_CFG.RF_Channel > 0x1F)return 1;	
	cnt = 200;
	while(RF_AUX_READ == 0){
		delay_ms(10);
		cnt--;
		if(cnt == 0){
			RF_MODE = RF_TRANS_MODE;
			return 2;
		}
	};
	RF_MOUDLE_SLEEP;
	delay_ms(100);
	cnt = 200;
	while(RF_AUX_READ == 0){
		delay_ms(10);
		cnt--;
		if(cnt == 0){
			RF_MODE = RF_TRANS_MODE;
			RF_MOUDLE_STANBY;
			return 2;
		}
	};
	RF_MODE = RF_CMD_MODE;
	RF_RX_Buf.Status = UART_READY;
	for(i = 0 ;i < 6; i ++){
		UART_Tx_Byte(RF_UART,((u8 *)&E32_CFG)[i]); 
	}
	cnt = 20;
	while(1){
		if(RF_RX_Buf.Status == UART_END)
			break;
		if(cnt == 0){
			RF_MODE = RF_TRANS_MODE;
			RF_MOUDLE_STANBY;
			return 2;
		}
		delay_ms(10);
		cnt--;
	}
	//RF_RX_Buf.Status = UART_READY;
	if(memcmp(RF_RX_Buf.data,(u8 *)&E32_CFG,sizeof(E32_CFG)) == 0){
		RF_MODE = RF_TRANS_MODE;
		
		UART_Tx_Byte(RF_UART,0xc4); 
		UART_Tx_Byte(RF_UART,0xc4); 
		UART_Tx_Byte(RF_UART,0xc4); 
		delay_ms(100);
		RF_MOUDLE_STANBY;
		return 0;
	}else{
		RF_MODE = RF_TRANS_MODE;
		RF_MOUDLE_STANBY;
		return 3;
	}
}


/*
* @brief  RF ��������һ��ָ��
  * @param  ��
  * @retval cmd ������  datalen ���ݳ��� data����
*/
void bsp_RF_UART_TX(u8 Cmd,u8 Datalen,u8*data)
{
	u8 i,sum;
	UART_Tx_Byte(RF_UART,UART_HEAD1);
	UART_Tx_Byte(RF_UART,UART_HEAD2);
	UART_Tx_Byte(RF_UART,Datalen + 5);
	UART_Tx_Byte(RF_UART,Cmd);
	sum  = Cmd;
	for(i = 0 ; i < Datalen ; i++){
		sum+=data[i];
		UART_Tx_Byte(RF_UART,data[i]);
	}
	UART_Tx_Byte(RF_UART,sum);
}


/*
* @brief  RF ��������һ��ָ���Ҫ���ذ���
  * @param  ��
  * @retval 0 �ɹ�  1 ʧ��  2:Rfģ��æ
*/
u8 bsp_RF_Send_Cmd(u8 Cmd,u32 Datalength,u8*data)
{
	u8 Cnt;
	Cnt = 200;
	while(RF_AUX_READ == 0){
		delay_ms(10);
		Cnt--;
		if(Cnt == 0){
			RF_MODE = RF_TRANS_MODE;
			return 2;
		}
	};
	bsp_RF_UART_TX(Cmd,Datalength,data);
	delay_ms(10);
	Cnt = 20;
	while(1){
		if(RF_RX_Buf.Status == UART_END)
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
* @brief  RF ���ڽ������ݽ�������
  * @param  ��
  * @retval �յ���ָ��
*/
u16 bsp_RF_UART_Parse(void)
{
	if(RF_RX_Buf.Status == UART_END){
		RF_RX_Buf.Status = UART_READY;
		if(RF_RX_Buf.Datalen< 2 |RF_RX_Buf.Datalen > DATA_MAX)return 0xffff;		
		memcpy(&RF_RX_CMD_Buf,RF_RX_Buf.data+2,RF_RX_Buf.Datalen-2);		
			//�����������
		switch(RF_RX_CMD_Buf.Cmd){
			case RF_GET_STATUS:
				STM_TX_Robot_Info(); 
				break;
			case UART_ACK:
				break;
		
			case RF_CTRL_MOTOR1_CMD:
				 bsp_RF_Send_Cmd(UART_ACK,0,0);
				 Motor1_Ctrl((int16_t)RF_RX_CMD_Buf.data[0]+(RF_RX_CMD_Buf.data[1]<<8));
				break;
			case RF_CTRL_MOTOR2_CMD:
				 bsp_RF_Send_Cmd(UART_ACK,0,0);
				 Motor2_Ctrl((int16_t)RF_RX_CMD_Buf.data[0]+(RF_RX_CMD_Buf.data[1]<<8));
				break;
			
			default:
				break;
		}
	}
	return RF_RX_CMD_Buf.Cmd;
}
