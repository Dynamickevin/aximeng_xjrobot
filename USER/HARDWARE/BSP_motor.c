#include "BSP_MOTOR.h"


/***
	***********************************************************************
	*							�����������������
	*
	*			SW1 --->  EN
	*			SW2 --->  DIR
	*			PWM ���Ʒ�ʽ	80Hz
	*
	***********************************************************************
***/

MOTOR_CTRL_TYPEDRF Motor1_CTRL,Motor2_CTRL;


static void bsp_Motor1_Config(void);
static void bsp_Motor2_Config(void);
static void bsp_Motor_GPIO_Init(void);

/*
  * @brief  MOTOR��ʼ��
  * @param  ��
  * @retval ��
*/
void bsp_motor_Init(void)
{
	bsp_Motor_GPIO_Init();
	bsp_Motor1_Config();
	bsp_Motor2_Config();
	bsp_motor1_cmd(DISABLE);
	bsp_motor2_cmd(DISABLE);
}
/*
**����������ų�ʼ������
*/
static void bsp_Motor_GPIO_Init(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (MOTOR1_EN_GPIO_CLK|MOTOR1_DIR_GPIO_CLK|MOTOR1_OCPWM_GPIO_CLK|MOTOR2_EN_GPIO_CLK|MOTOR2_DIR_GPIO_CLK\
							|MOTOR2_OCPWM_GPIO_CLK, ENABLE); 
 															   

	GPIO_SetBits(MOTOR1_EN_GPIO_PORT,MOTOR1_EN_PIN);
	GPIO_SetBits(MOTOR1_DIR_GPIO_PORT,MOTOR1_DIR_PIN);
	GPIO_SetBits(MOTOR2_EN_GPIO_PORT,MOTOR2_EN_PIN);
	GPIO_SetBits(MOTOR2_DIR_GPIO_PORT,MOTOR2_DIR_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Pin = MOTOR1_EN_PIN;		
	GPIO_Init(MOTOR1_EN_GPIO_PORT, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = MOTOR1_DIR_PIN;		
	GPIO_Init(MOTOR1_DIR_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = MOTOR2_EN_PIN;		
	GPIO_Init(MOTOR2_EN_GPIO_PORT, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = MOTOR2_DIR_PIN;		
	GPIO_Init(MOTOR2_DIR_GPIO_PORT, &GPIO_InitStructure);	

	/* ��ʱ��ͨ�����Ÿ��� */
	GPIO_PinAFConfig(MOTOR1_OCPWM_GPIO_PORT,MOTOR1_OCPWM_PINSOURCE,MOTOR1_OCPWM_AF); 
	GPIO_PinAFConfig(MOTOR2_OCPWM_GPIO_PORT,MOTOR2_OCPWM_PINSOURCE,MOTOR2_OCPWM_AF); 
	/* ��ʱ��ͨ���������� */															   
	GPIO_InitStructure.GPIO_Pin = MOTOR1_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(MOTOR1_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MOTOR2_OCPWM_PIN;	
	GPIO_Init(MOTOR2_OCPWM_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 */
static void bsp_Motor1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
	RCC_APB2PeriphClockCmd(MOTOR1_TIM_CLK, ENABLE); 


	TIM_TimeBaseStructure.TIM_Period = MOTOR1_TIM_Period-1;       

	TIM_TimeBaseStructure.TIM_Prescaler = MOTOR1_TIM_PSC-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	 
	TIM_TimeBaseInit(MOTOR1_TIM, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	MOTOR1_TIM_OC_INIT(MOTOR1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1

	// ʹ�ܶ�ʱ��
	TIM_Cmd(MOTOR1_TIM, ENABLE);	
	TIM_CtrlPWMOutputs(MOTOR1_TIM,ENABLE);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 */
static void bsp_Motor2_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
	RCC_APB2PeriphClockCmd(MOTOR2_TIM_CLK, ENABLE); 


	TIM_TimeBaseStructure.TIM_Period = MOTOR2_TIM_Period-1;       

	TIM_TimeBaseStructure.TIM_Prescaler = MOTOR2_TIM_PSC-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	 
	TIM_TimeBaseInit(MOTOR2_TIM, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 20;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	MOTOR2_TIM_OC_INIT(MOTOR2_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  

	// ʹ�ܶ�ʱ��
	TIM_Cmd(MOTOR2_TIM, ENABLE);	
	TIM_CtrlPWMOutputs(MOTOR2_TIM,ENABLE);
}



/*
  * @brief  ���1 ����ֹͣ����
  * @param  ENABLE: ����  DISABLE: �ر�  	
  * @retval ��
*/
void bsp_motor1_cmd(FunctionalState NewState)
{
	if(NewState == ENABLE){
		TIM_Cmd(MOTOR1_TIM, ENABLE);
		GPIO_ResetBits(MOTOR1_EN_GPIO_PORT,MOTOR1_EN_PIN);
	}else{
		TIM_Cmd(MOTOR1_TIM, DISABLE);
		GPIO_SetBits(MOTOR1_EN_GPIO_PORT,MOTOR1_EN_PIN);
	}
}
/*
  * @brief  ���2 ����ֹͣ����
  * @param  ENABLE: ����  DISABLE: �ر�  	
  * @retval ��
*/
void bsp_motor2_cmd(FunctionalState NewState)
{
	if(NewState == ENABLE){
		TIM_Cmd(MOTOR2_TIM, ENABLE);
		GPIO_ResetBits(MOTOR2_EN_GPIO_PORT,MOTOR2_EN_PIN);
	}else{
		TIM_Cmd(MOTOR2_TIM, DISABLE);
		GPIO_SetBits(MOTOR2_EN_GPIO_PORT,MOTOR2_EN_PIN);
	}
}

/*
  * @brief  ���1 �ٶȵ���
  * @param  0~9000;  	
  * @retval ��
*/
void bsp_motor1_Set_Speed(u16 NewSpeed)
{
	if(NewSpeed>MOTOR1_TIM_Period*0.9)NewSpeed = MOTOR1_TIM_Period*0.9;
	MOTOR1_SetCompare(MOTOR1_TIM,NewSpeed);	
}

/*
  * @brief  ���2 �ٶȵ���
  * @param  1000~9000;  	
  * @retval ��
*/
void bsp_motor2_Set_Speed(u16 NewSpeed)
{
	if(NewSpeed>MOTOR1_TIM_Period*0.9)NewSpeed = MOTOR1_TIM_Period*0.9;
	MOTOR2_SetCompare(MOTOR2_TIM,NewSpeed);	
}

/*
  * @brief  ���1 ������ת����
  * @param  1000~9000;  	
  * @retval ��
*/
void bsp_motor1_Set_Dir(MOTOR_DIR NewDir)
{
	if(NewDir == MOTOR_CW){
		GPIO_SetBits(MOTOR1_DIR_GPIO_PORT,MOTOR1_DIR_PIN);	
	}else{
		GPIO_ResetBits(MOTOR1_DIR_GPIO_PORT,MOTOR1_DIR_PIN);	
	}
}

/*
  * @brief  ���2 ������ת����
  * @param  0~9000;  	
  * @retval ��
*/
void bsp_motor2_Set_Dir(MOTOR_DIR NewDir)
{
	if(NewDir == MOTOR_CW){
		GPIO_SetBits(MOTOR2_DIR_GPIO_PORT,MOTOR2_DIR_PIN);	
	}else{
		GPIO_ResetBits(MOTOR2_DIR_GPIO_PORT,MOTOR2_DIR_PIN);	
	}
}

void Motor1_Ctrl(int16_t Speed)
{
	u16 Speed_New;	
	if(Speed < 0){		
		Speed_New = 0 - Speed;
		bsp_motor1_Set_Speed(Speed_New);
		bsp_motor1_Set_Dir(MOTOR_CCW);
		bsp_motor1_cmd(ENABLE);	
	}else if(Speed > 0){
		Speed_New = Speed;
		bsp_motor1_Set_Dir(MOTOR_CW);
		bsp_motor1_Set_Speed(Speed_New);
		bsp_motor1_cmd(ENABLE);
	}else {
		bsp_motor1_cmd(DISABLE);
	}
	
	
}

void Motor2_Ctrl(int16_t Speed)
{u16 Speed_New;	
	if(Speed < 0){		
		Speed_New = 0 - Speed;
		bsp_motor2_Set_Speed(Speed_New);
		bsp_motor2_Set_Dir(MOTOR_CCW);
		bsp_motor2_cmd(ENABLE);	
	}else if(Speed > 0){
		Speed_New = Speed;
		bsp_motor2_Set_Dir(MOTOR_CW);
		bsp_motor2_Set_Speed(Speed_New);
		bsp_motor2_cmd(ENABLE);
	}else {
		bsp_motor2_cmd(DISABLE);
	}
}

