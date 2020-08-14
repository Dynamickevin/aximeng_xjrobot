#ifndef 	_BSP_LIMIT_SW_H_
#define 	_BSP_LIMIT_SW_H_


#include "stm32f4xx.h"


//#define EXTI_ON			//ʹ���жϷ�ʽ��ȡ����״̬	

//�ж����ȼ�   (1��0) (1��1) (1��2)

//���Ŷ���
/*******************************************************/
#define LIMIT_SW1_PIN                  	GPIO_Pin_4                 
#define LIMIT_SW1_GPIO_PORT            	GPIOC         
#define GPIO_CHK_LIMIT1   		 					GPIOC,GPIO_Pin_4     //�Ӷ����˶�������λ���ؼ��1
#define LIMIT_SW1_GPIO_CLK            	RCC_AHB1Periph_GPIOC
#define Limit_SW1_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)

#define LIMIT_SW2_PIN                  	GPIO_Pin_5                 
#define LIMIT_SW2_GPIO_PORT            	GPIOC  
#define GPIO_CHK_LIMIT2   							GPIOC,GPIO_Pin_5     //�Ӷ����˶�������λ���ؼ��2
#define LIMIT_SW2_GPIO_CLK             	RCC_AHB1Periph_GPIOC
#define Limit_SW2_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

#define NEST_SW_PIN                  	GPIO_Pin_13                 
#define NEST_SW_GPIO_PORT            	GPIOC 
#define GPIO_CHK_CHAOXUE1	 	 					GPIOC,GPIO_Pin_13	  //���볲Ѩ���ؼ��1
#define NEST_SW_GPIO_CLK             	RCC_AHB1Periph_GPIOC
#define	NEST_SW_ReadBit()							GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)




//�ж����
#ifdef	EXTI_ON

//Ĭ�϶������±��ش���
#define	LIMIT_SW1_EXTI_PORT				EXTI_PortSourceGPIOC		//�ж��߶˿� EXTI_PortSourceGPIOX(X = A,B,C,D..)
#define	LIMIT_SW1_EXTI_PIN				EXTI_PinSource4				//�ж���IO	 EXTI_PinSourceX(X = 0~15)
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
/*	LIMIT_SW_NOTHING		û���κ��¼�
**	LIMIT_SW_RISING			�����ش���		��һ��״̬Ϊ�͵�ƽ����ǰΪ�ߵ�ƽ
**	LIMIT_SW_FALLTHING		�����ش���		��һ��״̬Ϊ�ߵ�ƽ����ǰΪ�͵�ƽ
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

