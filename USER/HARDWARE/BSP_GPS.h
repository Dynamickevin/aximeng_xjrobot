#ifndef 	_GPS_GPS_H_
#define 	_GPS_GPS_H_


/**
  *
  *		调用初始化GPS函数后   只需调用bsp_Get_GPS_Info(&GPS_Info);（GPS_Info需用户自己定义）即可获得GPS信息
  *
**/



#include "stm32f4xx.h"

#define GPS_BDS    //BDS 北斗导航系统



//引脚定义
/*******************************************************/
#define GPS_EN_PIN                  GPIO_Pin_8                 
#define GPS_EN_GPIO_PORT            GPIOC                      
#define GPS_EN_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define GPS_TTL_UART                             UART4
#define GPS_TTL_UART_CLK                         RCC_APB1Periph_UART4

#define GPS_TTL_UART_BAUDRATE                    9600	

#define GPS_TTL_UART_RX_GPIO_PORT                GPIOC
#define GPS_TTL_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define GPS_TTL_UART_RX_PIN                      GPIO_Pin_10
#define GPS_TTL_UART_RX_AF                       GPIO_AF_UART4
#define GPS_TTL_UART_RX_SOURCE                   GPIO_PinSource10

#define GPS_TTL_UART_TX_GPIO_PORT                GPIOC
#define GPS_TTL_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define GPS_TTL_UART_TX_PIN                      GPIO_Pin_11
#define GPS_TTL_UART_TX_AF                       GPIO_AF_UART4
#define GPS_TTL_UART_TX_SOURCE                   GPIO_PinSource11

//#define GPS_TTL_UART_IRQHandler                  UART4_IRQHandler
//#define GPS_TTL_UART_IRQ                 				 UART4_IRQn
	

#define GPS_RS232_UART                             UART5
#define GPS_RS232_UART_CLK                         RCC_APB1Periph_UART5
//串口波特率
#define GPS_RS232_UART_BAUDRATE                    9600	

#define GPS_RS232_UART_RX_GPIO_PORT                GPIOC
#define GPS_RS232_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define GPS_RS232_UART_RX_PIN                      GPIO_Pin_12
#define GPS_RS232_UART_RX_AF                       GPIO_AF_UART5
#define GPS_RS232_UART_RX_SOURCE                   GPIO_PinSource12

#define GPS_RS232_UART_TX_GPIO_PORT                GPIOD
#define GPS_RS232_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GPS_RS232_UART_TX_PIN                      GPIO_Pin_2
#define GPS_RS232_UART_TX_AF                       GPIO_AF_UART5
#define GPS_RS232_UART_TX_SOURCE                   GPIO_PinSource2

#define GPS_RS232_UART_IRQ                 		  	 UART5_IRQn
#define GPS_RS232_UART_IRQHandler                  UART5_IRQHandler


typedef enum{GPS_TTL,GPS_RS232}GPS_PORT;

	
///GPS
/************************************************************/

#ifdef GPS_BDS

#define  GGAHeader  "GNGGA"	   
#define  RMCHeader  "GNRMC"	
#define  GSAHeader	"GNGSA"
#define  VTGHeader	"GNVTG"
#else
#define  GGAHeader  "GPGGA"	   
#define  RMCHeader  "GPRMC"	
#define  GSAHeader	"GPGSA"
#define  VTGHeader	"GPVTG"
#endif
/************************************************************/
//逗号之间的数据最大值
#define RDCNT_MAX        90 

typedef struct
{
	char h;			//时
	char m;			//分
	char s;			//秒
}time;

typedef  struct
{
	u8 		Status;						//GPS状态，0=未定位，1=非差分定位，2=差分定位，3=无效PPS，6=正在估算
	time  	NewTime;					//GPS时间
	u8		currentSatellitesUsed;		//所用星数
	double  currentLatitude;			//纬度
	double 	currentLongitude;			//经度
	float 	HDOP;						//水平精度因子
	float   Speed;						//速度
}GPS_Info_Typedef; 


void  bsp_GPS_Init(GPS_PORT  GPS_COM);
void  bsp_GPS_cmd(FunctionalState NewSta);

u8 bsp_GPS_Get_Info( GPS_Info_Typedef   *GPS_Info);

#endif

/**************************************************END LINE**********************************************/
