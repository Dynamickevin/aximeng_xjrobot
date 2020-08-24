
#ifndef __NVIC_IRQ_H
#define __NVIC_IRQ_H

#include "includes.h"







//#define LIMIT_SW1_PreemptionPriority								1							//��ռ���ȼ�
//#define	LIMIT_SW1_SubPriority										1							//�����ȼ�

//#define LIMIT_SW2_PreemptionPriority								1							//��ռ���ȼ�
//#define	LIMIT_SW2_SubPriority										2							//�����ȼ�

//#define NEST_SW_PreemptionPriority									1							//��ռ���ȼ�
//#define	NEST_SW_SubPriority											0							//�����ȼ�



#define GPS_TTL_UART_PreemptionPriority								1							//��ռ���ȼ�
#define	GPS_TTL_UART_SubPriority				 					3							//�����ȼ�

#define GPS_RS232_UART_PreemptionPriority		   					1							//��ռ���ȼ�
#define	GPS_RS232_UART_SubPriority				   					2							//�����ȼ�

//#define BRD_UART_PreemptionPriority									2							//��ռ���ȼ�
//#define	BRD_UART_SubPriority				 						1							//�����ȼ�

//#define RF_UART_PreemptionPriority									2							//��ռ���ȼ�
//#define	RF_UART_SubPriority				 							2							//�����ȼ�


#define HI3521_UART_PreemptionPriority								2							//��ռ���ȼ�
#define	HI3521_UART_SubPriority				 						3							//�����ȼ�


#define ENCCODER_AB_TIM_PreemptionPriority		   					0							//��ռ���ȼ�
#define	ENCCODER_AB_TIM_SubPriority				   					3							//�����ȼ�

#define ENCCODER_Z_PreemptionPriority								0							//��ռ���ȼ�
#define	ENCCODER_Z_SubPriority										4							//�����ȼ�




//#define GENERAL_TIM_PreemptionPriority		   						3							//��ռ���ȼ�
//#define	GENERAL_TIM_SubPriority				   						3							//�����ȼ�






void NVIC_Configuration(void);

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
