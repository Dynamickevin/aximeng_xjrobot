

#ifndef 	_BSP_BRD_H_
#define 	_BSP_BRD_H_

/**
  *
  *		电源控制板 
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

//命令字
/****************************************************************************************************/
//数据方向
//#define		STM32_HI3521_CMD1				0x1200 	//STM32 -->HI35213521
#define		STM32_BRD_CMD1					0x1300	//STM32 -->BRD
//#define		HI3521_STM32_CMD1				0x2100	//HI35213521-->STM32
#define		BRD_STM32_CMD1					0x3100	//BRD   -->STM32
//#define		STM32_RF_CMD1						0x1400	//STM32 -->RF
//#define 	RF_STM32_CMD1						0x4100	//RF   -->STM32

//#define		HI3521_BRD_CMD1					0x3200	//HI35213521-->BRD
//#define		BRD_HI3521_CMD1					0x2300	//BRD   -->HI35213521


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


////STM32 -->HI35213521
//#define		STM_TX_CTRL_YT_CMD					0x02
//#define		STM_TX_CTRL_VIDEO_TX_CMD		0x03


////HI35213521-->STM32
//#define		HI3521_TX_GET_STATUS						0x04
//#define		HI3521_RX_REPLY_STATUS					0x04
//#define		HI3521_TX_CTRL_CMD							0x05
/****************************************************************************************************/

/********************************************************/

//机器人主板与电源板内部通讯协议  版本1.1.1 

#define			REPLY_GET_STATUS			 0x81 					//获取设备状态回复指令
#define			REPLY_GET_PROTECT			 0x82 					//获取保护参数回复指令
#define			REPLY_SET_PW					 0x83 					//控制电源状态回复指令

#define			ACK			    	 				 0x00 					//通用应答指令
#define			GET_STATUS			    	 0x01 					//获取设备状态指令
#define			GET_PROTECT_CMD			   0x02 					//获取保护参数指令
#define			SET_PW_STATUS			     0x03 					//控制电源状态指令
#define			SET_CHARGE_STATUS      0x04 					//控制充电开关指令
#define			SET_CELL_BALANCE    	 0x05 					//控制平衡开关指令



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


//帧格式内部数据包 
typedef struct {
	u32 Packet_Length;		//包括从包头到校验和的所有字节长度，Data_Length为数据尺寸，Packet Length = Data_Length + 8
												//8：Checksum + Packet_Length + Cmd + UART_HEAD2 + UART_HEAD1
	u8  Cmd;							//定义具体业务报文命令
	u8  Data[DATA_MAX];
	u8  Checksum;					//包括包头到数据区所有数据的按字节的求和值，取低8位
	
}Power_UARTbuf_Typedef;




/************************************************************************************/


/************************************************************************************/
//串口接收BUF
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
	u8 Expired_times;	//连续未收到数据包的次数
	u32 SendTime;
	u32 Last_Time;
}Pluse_Info_Typedef;
/************************************************************************************/


#define GPIO_BAT_CHARGE_EN      GPIOD,GPIO_Pin_1     //充电使能
#define GPIO_BREAK_MEN          GPIOE,GPIO_Pin_1     //抱闸开关；低电平使能

//充电检测引脚
#define GPIO_CHK_CHAG_SHORT	 	 GPIOE,GPIO_Pin_4    //充电短路检测
#define GPIO_CHK_CHAG_JOIN	 	 GPIOE,GPIO_Pin_5	   //充电接入检测

//引脚定义
/*******************************************************/

#define BRD_UART                             USART2
#define BRD_UART_CLK                         RCC_APB1Periph_USART2

#define BRD_UART_BAUDRATE                    57600	

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
	u16					Total_voltage;  				//总电压(mV)，无符号数16bit
	int16_t			Electric_current;				//总电流(mA)，有符号数16bit
	int16_t			BTemp[2]; 							//2路温度x10 （有符号）
	u16 				Monomer_voltage[6]; 		//6 个单体的电压信息(mV)，无符号数16bit
	u8					Sw_Status;							//4 路开关信息（低4bit 按位，1 代表打开，0 代表关闭）
	u8          BMS_Protect_Status;			//BMS 保护故障标志
	u8					Change_MOS_Status;			//充放电MOS 状态标志,（低2bit 按位，1 代表打开，0 代表关闭）
	u8					Battery_Balance_Status; //是否启用电池平衡功能,，0x01 代表启用平衡，0x00 代表关闭平衡功能。
	int16_t			Accel_Value_X;		//X 轴方向加速度值，单位为0.1m/s2
	int16_t			Accel_Value_Y;		//Y 轴方向加速度值，单位为0.1m/s2
	int16_t			Accel_Value_Z;		//Z 轴方向加速度值，单位为0.1m/s2
} BRD_Info_Typedef;


typedef struct {
	u8		Short_Protect_Current_threshold;  					//短路保护电流门限(A)，无符号数8bit
	u8		Overload_Protect_Current_threshold;					//过载保护电流门限(A)，无符号数8bit
	u16		Monomer_overvoltage_Protect_threshold; 			//单体过压保护门限(mV)，无符号数16bit
	u16 	Monomer_Undervoltage_Protect_threshold; 		//单体欠压保护门限(mV)，无符号数16bit
	u16		Discharge_capacity_Protect_threshold;				//放电容量保护门限(mAh)，无符号数16bit
} BAT_Protect_Info_Typedef;

typedef struct {
	u8		Power_SW_Status;  					//4 路开关控制（低4bit 按位，1 代表打开，0 代表关闭）
} Power_Control_Status_Typedef;


extern 	BRD_Info_Typedef 								BRD_Info;
extern	BAT_Protect_Info_Typedef				BAT_Protect_Info;
extern  Power_Control_Status_Typedef 		Power_Control_Status; 

extern BRD_Info_Typedef gBattery_Value;

extern UARTbuf_Typedef UART_Rx_Buf;
extern Power_UARTbuf_Typedef Power_UART_Rx_Buf;
u32 crc32( u32 inCrc32, const void *buf, u32 bufLen );
void UART_Send_Cmd(USART_TypeDef * UART , u16 CMD ,u32 Data_Length,u32 Param,u8 *data);
void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data);
 
extern RX_Buf_Typedef 			BRD_RX_Buf;
extern Pluse_Info_Typedef		BRD_Pluse_Info;

void  bsp_BRD_Init(void);
void bsp_BRD_Pluse(void);
u16 bsp_BRD_UART_Parse(void);

u16 bsp_Power_BRD_UART_Parse(void);

void STM_GET_POWER_Info(void);

short POWER_get_BAT_VOL(void);

short POWER_get_BAT_CUR(void);

#endif

