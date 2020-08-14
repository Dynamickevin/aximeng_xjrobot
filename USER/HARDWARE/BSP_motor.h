#ifndef 	_BSP_MOTOR_H_
#define 	_BSP_MOTOR_H_

#include "stm32f4xx.h"




//定义
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

#define	MOTOR1_TIM_Period			10000			//频率 = 系统频率（168M）/MOTOR1_TIM_PSC/MOTOR1_TIM_Period = 80HZ   分辨率为MOTOR1_TIM_Period
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
#define	MOTOR2_TIM_Period			10000			//频率 = 系统频率（168M）/MOTOR1_TIM_PSC/MOTOR1_TIM_Period   分辨率为MOTOR1_TIM_Period
#define	MOTOR2_TIM_PSC				210
#define	MOTOR2_SetCompare			TIM_SetCompare3
/*******************************************************/

typedef enum {MOTOR_CCW = 0,MOTOR_CW}MOTOR_DIR;

typedef struct{
	FunctionalState NewState;		//ENABLE / DISABLE
	int16_t 		Speed;			
}MOTOR_CTRL_TYPEDRF;

// motor控制调用函数
/*********************************************************/
void bsp_motor_Init(void);			//电机驱动初始化

void bsp_motor1_cmd(FunctionalState NewState);		//	
void bsp_motor2_cmd(FunctionalState NewState);		// 电机使能/失能命令

void bsp_motor1_Set_Speed(u16 NewSpeed);			//	
void bsp_motor2_Set_Speed(u16 NewSpeed);			// 电机速度设置  1000~9000

void bsp_motor1_Set_Dir(MOTOR_DIR NewDir);			//
void bsp_motor2_Set_Dir(MOTOR_DIR NewDir);			//电机方向设置    MOTOR_CCW 逆时针 MOTOR_CW顺时针
/************************/


void Motor1_Ctrl(int16_t Speed);	//	NewState// 电机使能/失能命令  Speed//-9000~9000 －为逆+为顺
void Motor2_Ctrl(int16_t Speed);


/*********************************************************/
#endif

