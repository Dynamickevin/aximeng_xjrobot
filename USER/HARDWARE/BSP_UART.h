#ifndef 	_BSP_UART_H_
#define 	_BSP_UART_H_


/**
  *
  *		
  *
**/

/*		*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
		*  	0x55(1)	*	0xaa(1)	*	CMD1(1)	*	CMD2(1)	*	Packet Length(4)							*
		*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
		*  Param1(4)									*	CRC32((4)									*
		*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
		*	Data(n)																						*
		*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*																								*
*/


#include "stm32f4xx.h"


#define  UART_HEAD1 0x55
#define  UART_HEAD2 0xaa

#define  UART_READY 	0
#define  UART_END		0x80

#define  RX_TIMEOUT		10;
#define DATA_MAX   512

//命令字
/****************************************************************************************************/
//数据方向
#define		STM32_HI3521_CMD1				0x1200 	//STM32 -->HI35213521
#define		STM32_BRD_CMD1					0x1300	//STM32 -->BRD
#define		HI3521_STM32_CMD1				0x2100	//HI35213521-->STM32
#define		BRD_STM32_CMD1					0x3100	//BRD   -->STM32
#define		STM32_RF_CMD1					0x1400	//STM32 -->RF
#define 	RF_STM32_CMD1					0x4100	//RF   -->STM32

#define		HI3521_BRD_CMD1					0x3200	//HI35213521-->BRD
#define		BRD_HI3521_CMD1					0x2300	//BRD   -->HI35213521


#define     UART_ACK						0x00
#define		STM_TX_GET_STAUS				0x01
#define		STM_RX_REPLY_STAUS				0x01


//STM32 -->HI35213521
#define		STM_TX_CTRL_YT_CMD				0x02
#define		STM_TX_CTRL_VIDEO_TX_CMD		0x02


//HI35213521-->STM32
#define		HI3521_TX_GET_STATUS					0x04
#define		HI3521_RX_REPLY_STATUS					0x04
#define		HI3521_TX_CTRL_CMD						0x05
/****************************************************************************************************/

/********************************************************/
//RF-->STM32 
#define	 RF_GET_STATUS								0x42
//#define	 RF_REPLY_STATUS								0x42
#define  RF_CTRL_MOTOR1_CMD							0x46
#define  RF_CTRL_MOTOR2_CMD							0x47




/********************************************************/



//  帧格式内部数据包 __PACKED紧凑结构 为方便传输
typedef struct __PACKED{
	u8 Cmd1;
	u8 Cmd2;
	u32 Packet_Length;		// Data_Length为数据尺寸，Packet Length = Data_Length+16
	u32	Param;
	u32 Crc32;
	u8 Data[DATA_MAX];
}UARTbuf_Typedef;
/************************************************************************************/


/************************************************************************************/
//串口接收BUF
typedef struct{
	u8 		Timeout;
	u8 		Status;
	u16     Index;
	u16		Datalen;
	u8 		data[DATA_MAX+32];
}RX_Buf_Typedef;
/************************************************************************************/

/************************************************************************************/
typedef struct{
	u8 Status;
	u8 Expired_times;	//连续未收到数据包的次数
	u32 SendTime;
	u32 Last_Time;
}Pluse_Info_Typedef;
/************************************************************************************/

extern UARTbuf_Typedef UART_Rx_Buf;



u32 crc32( u32 inCrc32, const void *buf, u32 bufLen );
void UART_Send_Cmd(USART_TypeDef * UART , u16 CMD ,u32 Data_Length,u32 Param,u8 *data);
void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data);

#endif

