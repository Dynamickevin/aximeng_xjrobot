
#ifndef __NVIC_IRQ_H
#define __NVIC_IRQ_H

#include "includes.h"







//#define LIMIT_SW1_PreemptionPriority								1							//抢占优先级
//#define	LIMIT_SW1_SubPriority										1							//次优先级

//#define LIMIT_SW2_PreemptionPriority								1							//抢占优先级
//#define	LIMIT_SW2_SubPriority										2							//次优先级

//#define NEST_SW_PreemptionPriority									1							//抢占优先级
//#define	NEST_SW_SubPriority											0							//次优先级



#define GPS_TTL_UART_PreemptionPriority								1							//抢占优先级
#define	GPS_TTL_UART_SubPriority				 					3							//次优先级

#define GPS_RS232_UART_PreemptionPriority		   					1							//抢占优先级
#define	GPS_RS232_UART_SubPriority				   					2							//次优先级

//#define BRD_UART_PreemptionPriority									2							//抢占优先级
//#define	BRD_UART_SubPriority				 						1							//次优先级

//#define RF_UART_PreemptionPriority									2							//抢占优先级
//#define	RF_UART_SubPriority				 							2							//次优先级


#define HI3521_UART_PreemptionPriority								2							//抢占优先级
#define	HI3521_UART_SubPriority				 						3							//次优先级


#define ENCCODER_AB_TIM_PreemptionPriority		   					0							//抢占优先级
#define	ENCCODER_AB_TIM_SubPriority				   					3							//次优先级

#define ENCCODER_Z_PreemptionPriority								0							//抢占优先级
#define	ENCCODER_Z_SubPriority										4							//次优先级




//#define GENERAL_TIM_PreemptionPriority		   						3							//抢占优先级
//#define	GENERAL_TIM_SubPriority				   						3							//次优先级






void NVIC_Configuration(void);

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
