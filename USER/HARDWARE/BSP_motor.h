#ifndef 	_BSP_MOTOR_H_
#define 	_BSP_MOTOR_H_

#include "stm32f4xx.h"




//����
/*******************************************************/
#define MOTOR1_EN_PIN          	GPIO_Pin_2              
#define MOTOR1_EN_GPIO_PORT    	GPIOB                      
#define MOTOR1_EN_GPIO_CLK      	RCC_AHB1Periph_GPIOB

#define MOTOR1_DIR_PIN          	GPIO_Pin_12              
#define MOTOR1_DIR_GPIO_PORT    	GPIOA                      
#define MOTOR1_DIR_GPIO_CLK      	RCC_AHB1Periph_GPIOA

#define MOTOR1_OCPWM_PIN          	GPIO_Pin_14             
#define MOTOR1_OCPWM_GPIO_PORT    	GPIOB                     
#define MOTOR1_OCPWM_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define MOTOR1_OCPWM_PINSOURCE		GPIO_PinSource14
#define MOTOR1_OCPWM_AF				GPIO_AF_TIM1

#define	MOTOR1_TIM           		TIM1
#define MOTOR1_TIM_CLK       		RCC_APB2Periph_TIM1

#define MOTOR1_TIM_OC_INIT			TIM_OC2Init

#define	MOTOR1_TIM_Period			10000			//Ƶ�� = ϵͳƵ�ʣ�168M��/MOTOR1_TIM_PSC/MOTOR1_TIM_Period = 80HZ   �ֱ���ΪMOTOR1_TIM_Period
#define	MOTOR1_TIM_PSC				210
#define	MOTOR1_SetCompare			TIM_SetCompare2


#define MOTOR2_EN_PIN          		GPIO_Pin_12              
#define MOTOR2_EN_GPIO_PORT    		GPIOB                      
#define MOTOR2_EN_GPIO_CLK      	RCC_AHB1Periph_GPIOB

#define MOTOR2_DIR_PIN          	GPIO_Pin_13              
#define MOTOR2_DIR_GPIO_PORT    	GPIOB                      
#define MOTOR2_DIR_GPIO_CLK      	RCC_AHB1Periph_GPIOB

#define MOTOR2_OCPWM_PIN          	GPIO_Pin_15             
#define MOTOR2_OCPWM_GPIO_PORT    	GPIOB                     
#define MOTOR2_OCPWM_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define MOTOR2_OCPWM_PINSOURCE		GPIO_PinSource15
#define MOTOR2_OCPWM_AF				GPIO_AF_TIM8


#define	MOTOR2_TIM           		TIM8
#define MOTOR2_TIM_CLK       		RCC_APB2Periph_TIM8

#define MOTOR2_TIM_OC_INIT			TIM_OC3Init
#define	MOTOR2_TIM_Period			10000			//Ƶ�� = ϵͳƵ�ʣ�168M��/MOTOR1_TIM_PSC/MOTOR1_TIM_Period   �ֱ���ΪMOTOR1_TIM_Period
#define	MOTOR2_TIM_PSC				210
#define	MOTOR2_SetCompare			TIM_SetCompare3
/*******************************************************/

typedef enum {MOTOR_CCW = 0,MOTOR_CW}MOTOR_DIR;

typedef struct{
	FunctionalState NewState;		//ENABLE / DISABLE
	int16_t 		Speed;			
}MOTOR_CTRL_TYPEDRF;

// motor���Ƶ��ú���
/*********************************************************/
void bsp_motor_Init(void);			//���������ʼ��

void bsp_motor1_cmd(FunctionalState NewState);		//	
void bsp_motor2_cmd(FunctionalState NewState);		// ���ʹ��/ʧ������

void bsp_motor1_Set_Speed(u16 NewSpeed);			//	
void bsp_motor2_Set_Speed(u16 NewSpeed);			// ����ٶ�����  1000~9000

void bsp_motor1_Set_Dir(MOTOR_DIR NewDir);			//
void bsp_motor2_Set_Dir(MOTOR_DIR NewDir);			//�����������    MOTOR_CCW ��ʱ�� MOTOR_CW˳ʱ��
/************************/


void Motor1_Ctrl(int16_t Speed);	//	NewState// ���ʹ��/ʧ������  Speed//-9000~9000 ��Ϊ��+Ϊ˳
void Motor2_Ctrl(int16_t Speed);


/*********************************************************/
#endif

