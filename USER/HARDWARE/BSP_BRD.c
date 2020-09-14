
#include <includes.h>

#include <math.h>
#include "string.h"
/************************************************************************/
/*********************************电源板控制*****************************/
/**/
/**/
/**/
/************************************************************************/

OS_EVENT	*Uart2Sem;					//定义电源板信息  信号量

 
/*
  * @brief  串口初始化函数
  * @param  无
  * @retval 无
*/


const u32 crcTable[256] = {
   0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
   0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
   0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
   0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
   0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
   0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
   0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
   0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
   0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
   0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
   0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
   0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
   0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
   0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
   0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
   0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
   0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
   0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
   0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
   0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
   0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
   0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
   0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
   0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
   0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
   0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
   0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
   0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
   0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
   0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
   0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
   0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
   0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
   0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
   0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
   0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
   0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D };
 
UARTbuf_Typedef UART_Sendbuf;
UARTbuf_Typedef UART_Rx_Buf;   


void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    USART_SendData(USARTx, Data);
		//掉电、待机等特殊情况下，保证USART数据会成功发送出去
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

  
void UART_Send_Cmd(USART_TypeDef * UART , u16 CMD ,u32 Data_Length,u32 Param,u8 *data)
{
	u32 Len;
	UART_Sendbuf.Crc32 = 0;
	UART_Sendbuf.Packet_Length = Data_Length + 16;
	UART_Sendbuf.Cmd1 = CMD >>8;
	UART_Sendbuf.Cmd2 = CMD & 0xff;
	UART_Sendbuf.Param = Param;
	if(Data_Length && data)memcpy(&UART_Sendbuf.Data[0],data,Data_Length);

	UART_Sendbuf.Crc32 = crc32(0,(void *)&UART_Sendbuf,UART_Sendbuf.Packet_Length-2);
	UART_Tx_Byte(UART,UART_HEAD1);
	UART_Tx_Byte(UART,UART_HEAD2);
	for(Len = 0 ; Len < UART_Sendbuf.Packet_Length - 2 ;Len++ )//2表示去掉帧头55aa
	{
		UART_Tx_Byte(UART,((u8 *)&UART_Sendbuf)[Len]);	
	}
}



u32 crc32( u32 inCrc32, const void *buf, u32 bufLen )
{
    u32 crc32;
    u8 *byteBuf;
    u32 i;

    /** accumulate crc32 for buffer **/
    crc32 = inCrc32 ^ 0xFFFFFFFF;
    byteBuf = (u8*) buf;
    for (i=0; i < bufLen; i++) {
        crc32 = (crc32 >> 8) ^ crcTable[ (crc32 ^ byteBuf[i]) & 0xFF ];
    }
    return( crc32 ^ 0xFFFFFFFF );
}



/*
  * @brief  BRD初始化函数，BRD在使用前必须先调用此函数
  * @param  无
  * @retval 无
*/

BRD_Info_Typedef BRD_Info; 
RX_Buf_Typedef BRD_RX_Buf;

Pluse_Info_Typedef BRD_Pluse_Info = {0};

void  bsp_BRD_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 UART2 TX RX  I/O口 时钟 */
	RCC_AHB1PeriphClockCmd( BRD_UART_RX_GPIO_CLK|BRD_UART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART2 时钟 */
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

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(BRD_UART, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */

	/*配置串口接收中断*/
	USART_ITConfig(BRD_UART, USART_IT_RXNE, ENABLE);

	Uart2Sem = OSSemCreate(1);
	
	USART_Cmd(BRD_UART, ENABLE);
	
}
/*
  * @brief  BRD 串口中断处理函数
  * @param  无
  * @retval 无
*/
void BRD_POWER_ISR(void)
{
	u8 tmp;
	if(USART_GetITStatus(BRD_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(BRD_UART);
		if(BRD_RX_Buf.Status == UART_END)
		{
			return;
		}
		else if(BRD_RX_Buf.Status == UART_READY)
		{
			BRD_RX_Buf.Index = 0;
		}
		BRD_RX_Buf.Timeout = RX_TIMEOUT;
		BRD_RX_Buf.data[BRD_RX_Buf.Index] =tmp;
		switch(BRD_RX_Buf.Status)
		{
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
				if(BRD_RX_Buf.Index == 7)
				{
					BRD_RX_Buf.Datalen = *(u16*)(BRD_RX_Buf.data+4);
					if(BRD_RX_Buf.Datalen < DATA_MAX + 16)
						BRD_RX_Buf.Status = 3;
					else BRD_RX_Buf.Status = 0;
				}
				break;
			case  3:
				if(BRD_RX_Buf.Index >= BRD_RX_Buf.Datalen - 1)
				{
					//BRD_Pluse_Info.Last_Time = Time100ms;
					BRD_RX_Buf.Status = UART_END;
					BRD_Pluse_Info.Status = 0;
					BRD_Pluse_Info.Expired_times = 0;
				}
				OSSemPost(Uart2Sem);
				break;
			default:
				break;
		}		
		BRD_RX_Buf.Index++;		
	}
	//LED2(LED_ON);
}


/*
* @brief  向电源板发送一条指令
  * @param  无
  * @retval cmd 命令字  datalen 数据长度 data数据
*/
//bsp_Power_UART_TX(STM_TX_GET_MST_BRAKE_STATUS,1,0)
//bsp_Power_UART_TX(STM_TX_GET_BAT_STATUS,0,0) //向电源板发送获取电池状态
//bsp_Power_UART_TX(STM_TX_GET_POWER_SW_STATUS,0,0) //向电源板发送获取设备电源状态

void bsp_Power_UART_TX(u8 Cmd,u8 Datalen,u8*data)
{
	u8 i,sum;
	UART_Tx_Byte(BRD_UART,UART_HEAD1);
	UART_Tx_Byte(BRD_UART,UART_HEAD2);
	UART_Tx_Byte(BRD_UART,Datalen + 5);
	UART_Tx_Byte(BRD_UART,Cmd);
	sum  = Cmd;
	for(i = 0 ; i < Datalen ; i++)
	{
		sum+=data[i];
		UART_Tx_Byte(BRD_UART,data[i]);
	}
	UART_Tx_Byte(BRD_UART,sum);
}

/*
* @brief  BRD 主动发送一条指令（需要返回包）
  * @param  无
  * @retval 0 成功  1 失败
*/
u8 bsp_BRD_Send_Cmd(u8 Cmd,u32 Datalength,u32 param,u8*data)
{
	u8 Cnt;
	uint8 err; 
  OSSemPend(Uart2Sem,0,&err);
	
	//debug_sprintf(ID_DEBUG,"1111");
	UART_Send_Cmd(BRD_UART,STM32_BRD_CMD1|Cmd,Datalength,param,data);
	delay_ms(10);
	Cnt = 20;
	while(1)
	{
		if(BRD_RX_Buf.Status == UART_END)
			break;	
		delay_ms(10);
		Cnt--;
		if(Cnt == 0)
		{
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
//void bsp_BRD_Pluse(void)
//{
//	if(Time100ms - BRD_Pluse_Info.Last_Time > 100){
//		if(BRD_Pluse_Info.Status != 0 )BRD_Pluse_Info.Expired_times ++;
//		UART_Send_Cmd(BRD_UART,STM32_BRD_CMD1|STM_TX_GET_STAUS,0,0,0);
//		BRD_Pluse_Info.Status = 1;
//		BRD_Pluse_Info.Last_Time = Time100ms;
//	}
//}


/*
* @brief  BRD 串口接收数据解析函数
  * @param  无
  * @retval 收到的指令
	
*/
u16 bsp_BRD_UART_Parse(void)
{
	u32 CRC32_Value;
	if(BRD_RX_Buf.Status == UART_END)
	{			
		memcpy(&UART_Rx_Buf,BRD_RX_Buf.data+2,BRD_RX_Buf.Datalen-2);//2表示去掉帧头55aa
		BRD_RX_Buf.Status = UART_READY;
		CRC32_Value = UART_Rx_Buf.Crc32;
		
		UART_Rx_Buf.Crc32 = 0 ;
		UART_Rx_Buf.Crc32 = crc32(0,(void *)&UART_Rx_Buf,UART_Rx_Buf.Packet_Length - 2);//2表示去掉帧头55aa
		
		if(CRC32_Value == UART_Rx_Buf.Crc32)
		{
			if(UART_Rx_Buf.Cmd1 == 0x31)			//0x31:表示电源板向主控板32发送消息
			{
				//处理接收数据
				switch(UART_Rx_Buf.Cmd2)
				{
					case STM_TX_GET_BAT_STATUS:
						memcpy(&BRD_Info,UART_Rx_Buf.Data,sizeof(BRD_Info));
						break;
					case UART_ACK:
						break;
					default:
						break;
				}
			}
			else
			{
			
			}		
		}
	}
	return UART_Rx_Buf.Cmd2;
}


//获取电源板信息参数
void STM_GET_POWER_Info(void)
{

	bsp_BRD_Send_Cmd(STM_TX_GET_BAT_STATUS,sizeof(BRD_Info),0,(u8 *)(&BRD_Info));	
	
}



