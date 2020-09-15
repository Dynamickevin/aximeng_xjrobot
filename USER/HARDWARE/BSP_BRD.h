

#ifndef 	_BSP_BRD_H_
#define 	_BSP_BRD_H_

/**
  *
  *		��Դ���ư� 
  *
**/

#include "stm32f4xx.h"
#include  "board.h"



#define  UART_HEAD1 	0x55
#define  UART_HEAD2 	0xaa

#define  UART_READY 	0
#define  UART_END			0x80

#define  RX_TIMEOUT		10
#define  DATA_MAX   	512

//������
/****************************************************************************************************/
//���ݷ���
#define		STM32_HI3521_CMD1				0x1200 	//STM32 -->HI35213521
#define		STM32_BRD_CMD1					0x1300	//STM32 -->BRD
#define		HI3521_STM32_CMD1				0x2100	//HI35213521-->STM32
#define		BRD_STM32_CMD1					0x3100	//BRD   -->STM32
#define		STM32_RF_CMD1						0x1400	//STM32 -->RF
#define 	RF_STM32_CMD1						0x4100	//RF   -->STM32

#define		HI3521_BRD_CMD1					0x3200	//HI35213521-->BRD
#define		BRD_HI3521_CMD1					0x2300	//BRD   -->HI35213521


#define   UART_ACK								0x00
#define		STM_TX_GET_STAUS				0x01
#define		STM_RX_REPLY_STAUS			0x01


//STM32 -->Power 
#define		STM_TX_GET_BAT_STATUS					0x13
#define		STM_TX_GET_MST_BRAKE_STATUS		0x12
#define		STM_TX_GET_POWER_SW_STATUS		0x11

//Power --> STM32
#define		POWER_RX_BAT_VALUE					0x21
#define		POWER_RX_POWER_SW_STATUS		0x23


//STM32 -->HI35213521
#define		STM_TX_CTRL_YT_CMD					0x02
#define		STM_TX_CTRL_VIDEO_TX_CMD		0x03


//HI35213521-->STM32
#define		HI3521_TX_GET_STATUS						0x04
#define		HI3521_RX_REPLY_STATUS					0x04
#define		HI3521_TX_CTRL_CMD							0x05
/****************************************************************************************************/

/********************************************************/
//RF-->STM32 
#define	 RF_GET_STATUS									0x42
//#define	 RF_REPLY_STATUS							0x42
#define  RF_CTRL_MOTOR1_CMD							0x46
#define  RF_CTRL_MOTOR2_CMD							0x47


/********************************************************/



//֡��ʽ�ڲ����ݰ� __PACKED���սṹ Ϊ���㴫��
typedef struct __PACKED{
	u8  Cmd1;							//���屨�ĵķ��ͺͽ��ն���
	u8  Cmd2;							//�������ҵ��������
	u32 Packet_Length;		//�����Ӱ�ͷ����β����֡���ȣ�Data_LengthΪ���ݳߴ磬Packet Length = Data_Length+16
												//16��Crc32 + Param + Packet_Length + Cmd2 +Cmd1 + UART_HEAD2 + UART_HEAD1
	u32	Param;						//���ݾ��������ֶ���
	u32 Crc32;						//������CMD1�����ݶε�CRC32У�飬CRC32�ֶΰ�0��������
	u8  Data[DATA_MAX];
}UARTbuf_Typedef;
/************************************************************************************/


/************************************************************************************/
//���ڽ���BUF
typedef struct{
	u8 		Timeout;
	u8 		Status;
	u16   Index;
	u16		Datalen;
	u8 		data[DATA_MAX+32];
}RX_Buf_Typedef;
/************************************************************************************/

/************************************************************************************/
typedef struct{
	u8 Status;
	u8 Expired_times;	//����δ�յ����ݰ��Ĵ���
	u32 SendTime;
	u32 Last_Time;
}Pluse_Info_Typedef;
/************************************************************************************/


#define GPIO_BAT_CHARGE_EN      GPIOD,GPIO_Pin_1     //���ʹ��
#define GPIO_BREAK_MEN          GPIOE,GPIO_Pin_1     //��բ���أ��͵�ƽʹ��

//���������
#define GPIO_CHK_CHAG_SHORT	 	 GPIOE,GPIO_Pin_4    //����·���
#define GPIO_CHK_CHAG_JOIN	 	 GPIOE,GPIO_Pin_5	   //��������

//���Ŷ���
/*******************************************************/

#define BRD_UART                             USART2
#define BRD_UART_CLK                         RCC_APB1Periph_USART2

#define BRD_UART_BAUDRATE                    115200	

#define BRD_UART_RX_GPIO_PORT                GPIOA
#define BRD_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_RX_PIN                      GPIO_Pin_2
#define BRD_UART_RX_AF                       GPIO_AF_USART2
#define BRD_UART_RX_SOURCE                   GPIO_PinSource2

#define BRD_UART_TX_GPIO_PORT                GPIOA
#define BRD_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_TX_PIN                      GPIO_Pin_3
#define BRD_UART_TX_AF                       GPIO_AF_USART2
#define BRD_UART_TX_SOURCE                   GPIO_PinSource3

#define BRD_UART_IRQHandler                  USART2_IRQHandler
#define BRD_UART_IRQ                 				 USART2_IRQn


typedef struct {
	u16			Total_voltage;  			//����ܵ�ѹ
	u16			Electric_current;			//��ص���
	int16_t BTemp[2]; 						//2·�¶�x10 ���з��ţ�
	u16 		Monomer_voltage[6]; 	//�����ѹ
	u8			Sw_Status;						//�豸��Դ����״̬
} BRD_Info_Typedef;

extern BRD_Info_Typedef gBattery_Value;

extern UARTbuf_Typedef UART_Rx_Buf;
u32 crc32( u32 inCrc32, const void *buf, u32 bufLen );
void UART_Send_Cmd(USART_TypeDef * UART , u16 CMD ,u32 Data_Length,u32 Param,u8 *data);
void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data);

extern BRD_Info_Typedef 		BRD_Info; 
extern RX_Buf_Typedef 			BRD_RX_Buf;
extern Pluse_Info_Typedef		BRD_Pluse_Info;

void  bsp_BRD_Init(void);
void bsp_BRD_Pluse(void);
u16 bsp_BRD_UART_Parse(void);

void STM_GET_POWER_Info(void);

short POWER_get_BAT_VOL(void);

short POWER_get_BAT_CUR(void);

#endif

