
#include "includes.h"
#include "stm32f4xx.h"                  // Device header file



//从动轮码盘参数定义
signed short CodeAB_Start;
char CodeZ_Start;
signed long gCodeZ;
signed long CodeAB_Last;

static void WHEEL_ENCODER_AB_Config(void);
volatile int32_t  Wheel_Enccoder_AB_Turns;
volatile int32_t  Wheel_Enccoder_Z_Turns;

void  wheel_encode_Init(void)
{
	wheel_encode_GPIO_Init();
	WHEEL_ENCODER_AB_Config();
	TimerCode_DefaultFunction_Init(4);  //从动轮轮子编码器 定时器、数据采集初始化 
	
}



//从动轮码盘的Z相初始化为外部中断
static void wheel_encode_GPIO_Init(void)
{
	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*开启wheel_encode相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd ( ENCCODER_A_GPIO_CLK|ENCCODER_B_GPIO_CLK|ENCCODER_Z_GPIO_CLK, ENABLE); 

	/*A、B IO口引脚映射定时器*/
	GPIO_PinAFConfig(ENCCODER_A_GPIO_PORT,ENCCODER_A_SOURCE, ENCCODER_A_AF);
	GPIO_PinAFConfig(ENCCODER_B_GPIO_PORT,ENCCODER_B_SOURCE, ENCCODER_B_AF);
	/*Z IO口引脚映射外部中断*/
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
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  													
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}


 /*
  * @brief  编码器 AB相TIM5初始化
  * @param  无
  * @retval 无
*/
static void WHEEL_ENCODER_AB_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

/*----------------------------------------------------------------*/        
  //使能定时器TIM5的时钟    
	RCC_APB1PeriphClockCmd(ENCCODER_AB_TIM_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period =200*4-1;       		//
	TIM_TimeBaseStructure.TIM_Prescaler =0;             //设置预分频：		
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;      //设置时钟分频系数：不分频
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
	Wheel_Enccoder_AB_Turns = 0;
	TIM_ClearFlag(ENCCODER_AB_TIM, TIM_FLAG_Update);
	TIM_ITConfig(ENCCODER_AB_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(ENCCODER_AB_TIM, ENABLE);	

}

/**************************************/
 /*
  * @brief  编码器 AB相TIM5定时器中断处理函数
  * @param  无
  * @retval 无
*/
void  WHEEL_ENCODER_AB_IRQHandler (void)
{
	if ( TIM_GetITStatus( ENCCODER_AB_TIM, TIM_IT_Update) != RESET ) 
	{	
		/* CR1  !< TIM control register 1,  Address offset: 0x00 */
		if(ENCCODER_AB_TIM->CR1 & 0X10)
		{
			Wheel_Enccoder_AB_Turns--;
		}
		else
		{
			Wheel_Enccoder_AB_Turns++;
		}
		TIM_ClearITPendingBit(ENCCODER_AB_TIM , TIM_IT_Update);  		 
	}		 	
}


//从动轮码盘的外部中断服务函数
void EXTI15_10_IRQ_ISR(void)
{
    signed short code;
	//signed long temp;//,CodeZ;
	//signed long CodeAB_Last;
	int16 Wheel_Code_Scale = 3000 ; //判断是否超过一圈

	code=GET_SLAVE_WHEEL_CODE();
	if ( EXTI_GetITStatus(EXTI_Line14) != RESET) 
	{    
    if((code-CodeAB_Last) > Wheel_Code_Scale)//正转一圈
    {   
			gCodeZ=gCodeZ+1;
    }
		else if((CodeAB_Last-code) > Wheel_Code_Scale)//反转一圈
    {
			gCodeZ=gCodeZ-1;
    }
		else
		{
		
		}
		CodeAB_Last=code;

		/*
		if(CodeZ_Start==1)		
		{
            code=GET_SLAVE_WHEEL_CODE()-CodeAB_Start;
			if(code<0)
			{
				temp=-code;
			}
 			temp=(temp+400)/800;
			CodeZ=temp;
			if(code<0)
			{
				GET_SLAVE_WHEEL_CODE()=-CodeZ*800+CodeAB_Start;
			}
			else
			{
				GET_SLAVE_WHEEL_CODE()=CodeZ*800+CodeAB_Start;
			}
		}
		else
		{
			CodeZ_Start=1;
			CodeAB_Start=GET_SLAVE_WHEEL_CODE();//记录第一个Z脉冲来临时的编码器的值
		}
		*/
		EXTI_ClearITPendingBit(EXTI_Line14);
	}						
}








