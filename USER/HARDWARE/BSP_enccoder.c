#include "BSP_enccoder.h"

/***
	***********************************************************************
	*							编码器驱动程序
	*
	***********************************************************************
***/


extern uint32_t Time100ms;

static void  bsp_enccoder_GPIO_Init(void);
static void ENCCODER_AB_Config(void);
volatile int32_t  Enccoder_AB_Turns;
volatile int32_t  Enccoder_Z_Turns;
/*
  * @brief  enccoder初始化
  * @param  无
  * @retval 无
*/
void  bsp_enccoder_Init(void)
{
	bsp_enccoder_GPIO_Init();
	ENCCODER_AB_Config();
}

/*
  * @brief  编码器 GPIO 初始化
  * @param  无
  * @retval 无
*/
static void  bsp_enccoder_GPIO_Init(void)
{

	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd ( ENCCODER_A_GPIO_CLK|ENCCODER_B_GPIO_CLK|ENCCODER_Z_GPIO_CLK, ENABLE); 

	GPIO_PinAFConfig(ENCCODER_A_GPIO_PORT,ENCCODER_A_SOURCE, ENCCODER_A_AF);
	GPIO_PinAFConfig(ENCCODER_B_GPIO_PORT,ENCCODER_B_SOURCE, ENCCODER_B_AF);
	
	SYSCFG_EXTILineConfig(ENCCODER_Z_EXTI_PORT  , ENCCODER_Z_EXTI_PIN  );
	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStructure.GPIO_Pin = ENCCODER_A_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(ENCCODER_A_GPIO_PORT, &GPIO_InitStructure);																   
	GPIO_InitStructure.GPIO_Pin = ENCCODER_B_PIN;	
	GPIO_Init(ENCCODER_B_GPIO_PORT, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Pin = ENCCODER_Z_PIN;	
	GPIO_Init(ENCCODER_Z_GPIO_PORT, &GPIO_InitStructure);	
	
	EXTI_InitStructure.EXTI_Line = ENCCODER_Z_EXTI_LINE;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
	

}

 /*
  * @brief  编码器 AB相TIM初始化
  * @param  无
  * @retval 无
*/
static void ENCCODER_AB_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

/*----------------------------------------------------------------*/        
      
	RCC_APB1PeriphClockCmd(ENCCODER_AB_TIM_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period =200*4-1;       //
	TIM_TimeBaseStructure.TIM_Prescaler =0;            //设置预分频：		
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(ENCCODER_AB_TIM, &TIM_TimeBaseStructure);
	
      
     /*-----------------------------------------------------------------*/
    //编码配置                        编码模式
	TIM_EncoderInterfaceConfig(ENCCODER_AB_TIM, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising上升沿捕获
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;         //比较滤波器
	TIM_ICInit(ENCCODER_AB_TIM, &TIM_ICInitStructure);
  
  //Reset counter
	ENCCODER_AB_TIM->CNT =0;	
	Enccoder_AB_Turns = 0;
	TIM_ClearFlag(ENCCODER_AB_TIM, TIM_FLAG_Update);
	TIM_ITConfig(ENCCODER_AB_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(ENCCODER_AB_TIM, ENABLE);	

}

 /*
  * @brief  编码器 获取编码值
  * @param  无
  * @retval 编码值
*/
 int32_t bsp_Enccoder_AB_GET_Cnt(void)
{
	return (ENCCODER_AB_TIM->CNT/4 + Enccoder_AB_Turns*200);
}



/**************************************/
 /*
  * @brief  编码器 AB相TIM定时器中断处理函数
  * @param  无
  * @retval 无
*/
void  ENCCODER_AB_IRQHandler (void)
{
	if ( TIM_GetITStatus( ENCCODER_AB_TIM, TIM_IT_Update) != RESET ) 
	{	
		if(ENCCODER_AB_TIM->CR1 & 0X10){
			Enccoder_AB_Turns--;
		}else{
			Enccoder_AB_Turns++;
		}
		TIM_ClearITPendingBit(ENCCODER_AB_TIM , TIM_IT_Update);  		 
	}		 	
}
 /*
  * @brief  编码器 Z，中断处理函数
  * @param  无
  * @retval 无
*/
void ENCCODER_Z_EXTI_Handler(void)
{
	static u8 dir_Down,dir_Up,Status;
	static u32 Time;
	if(EXTI_GetITStatus(ENCCODER_Z_EXTI_LINE) != RESET) 
	{
		if(GPIO_ReadInputDataBit(ENCCODER_Z_GPIO_PORT,ENCCODER_Z_PIN) == 0){
			dir_Down = ENCCODER_AB_TIM->CR1 & 0X10;
		}else{
			dir_Up = ENCCODER_AB_TIM->CR1 & 0X10;
			//if(dir_Up == dir_Down && Time <= Time100ms)
			if(dir_Up == dir_Down )
			{
				//Time = Time100ms + 2;
				if(dir_Up)
				{
					Enccoder_Z_Turns--;
				}
				else
				{
					Enccoder_Z_Turns++;
				}
			}			
		}
		//清除中断标志位
		EXTI_ClearITPendingBit(ENCCODER_Z_EXTI_LINE);     
	} 
}
