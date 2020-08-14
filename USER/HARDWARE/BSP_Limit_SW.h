#ifndef 	_BSP_LIMIT_SW_H_
#define 	_BSP_LIMIT_SW_H_


#include "stm32f4xx.h"


//#define EXTI_ON			//使用中断方式读取开关状态	

//中断优先级   (1，0) (1，1) (1，2)

//引脚定义
/*******************************************************/
#define LIMIT_SW1_PIN                  	GPIO_Pin_4                 
#define LIMIT_SW1_GPIO_PORT            	GPIOC         
#define GPIO_CHK_LIMIT1   		 					GPIOC,GPIO_Pin_4     //从动轮运动上下限位开关检测1
#define LIMIT_SW1_GPIO_CLK            	RCC_AHB1Periph_GPIOC
#define Limit_SW1_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)

#define LIMIT_SW2_PIN                  	GPIO_Pin_5                 
#define LIMIT_SW2_GPIO_PORT            	GPIOC  
#define GPIO_CHK_LIMIT2   							GPIOC,GPIO_Pin_5     //从动轮运动上下限位开关检测2
#define LIMIT_SW2_GPIO_CLK             	RCC_AHB1Periph_GPIOC
#define Limit_SW2_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

#define NEST_SW_PIN                  	GPIO_Pin_13                 
#define NEST_SW_GPIO_PORT            	GPIOC 
#define GPIO_CHK_CHAOXUE1	 	 					GPIOC,GPIO_Pin_13	  //进入巢穴开关检测1
#define NEST_SW_GPIO_CLK             	RCC_AHB1Periph_GPIOC
#define	NEST_SW_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)




//中断相关
#ifdef	EXTI_ON

//默认都是上下边沿触发
#define	LIMIT_SW1_EXTI_PORT				EXTI_PortSourceGPIOC		//中断线端口 EXTI_PortSourceGPIOX(X = A,B,C,D..)
#define	LIMIT_SW1_EXTI_PIN				EXTI_PinSource4				//中断线IO	 EXTI_PinSourceX(X = 0~15)
#define	LIMIT_SW1_EXTI_LINE				EXTI_Line4				
#define	LIMIT_SW1_IRQn					EXTI4_IRQn

#define	LIMIT_SW1_EXTI_HANDLER			EXTI4_IRQHandler

#define	LIMIT_SW2_EXTI_PORT				EXTI_PortSourceGPIOC
#define	LIMIT_SW2_EXTI_PIN				EXTI_PinSource5
#define	LIMIT_SW2_EXTI_LINE				EXTI_Line5
#define	LIMIT_SW2_IRQn					EXTI9_5_IRQn

//#define	LIMIT_SW2_EXTI_HANDLER			EXTI9_5_IRQHandler

#define	NEST_SW_EXTI_PORT				EXTI_PortSourceGPIOC
#define	NEST_SW_EXTI_PIN				EXTI_PinSource13
#define	NEST_SW_EXTI_LINE				EXTI_Line13
#define	NEST_SW_IRQn					EXTI15_10_IRQn

//#define	NEST_SW_EXTI_HANDLER			EXTI15_10_IRQHandler


/************************************************************/

typedef enum{LIMIT_SW_NOTHING = 0,LIMIT_SW_RISING,LIMIT_SW_FALLING}LIMIT_SW_Flag_Typedef;
/*	LIMIT_SW_NOTHING		没有任何事件
**	LIMIT_SW_RISING			上升沿触发		上一个状态为低电平，当前为高电平
**	LIMIT_SW_FALLTHING		下升沿触发		上一个状态为高电平，当前为低电平
*/
extern LIMIT_SW_Flag_Typedef	Limit_SW1_Flag,Limit_SW2_Flag,Nest_SW_Flag;
/**************************************************************/
#endif


/**************************************************************/

	

void  bsp_Limit_SW_Init(void);

#ifdef	EXTI_ON
void NEST_SW_EXTI_Handler(void);
void LIMIT_SW2_EXTI_Handler(void);
void LIMIT_SW1_EXTI_Handler(void);

LIMIT_SW_Flag_Typedef Limit_SW1_Get_Event(void);
LIMIT_SW_Flag_Typedef Limit_SW2_Get_Event(void);
LIMIT_SW_Flag_Typedef Nest_SW_Get_Event(void)  ;
#endif




#endif

