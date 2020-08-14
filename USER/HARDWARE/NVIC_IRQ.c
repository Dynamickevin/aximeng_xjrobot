
#include "NVIC_IRQ.h"


void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源

		//中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = LIMIT_SW1_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LIMIT_SW1_PreemptionPriority;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = LIMIT_SW1_SubPriority;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = LIMIT_SW2_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LIMIT_SW2_PreemptionPriority;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = LIMIT_SW2_SubPriority;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = NEST_SW_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NEST_SW_PreemptionPriority;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NEST_SW_SubPriority;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
		  /* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = GPS_TTL_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPS_TTL_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = GPS_TTL_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
			  /* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = GPS_RS232_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPS_TTL_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = GPS_TTL_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
				  /* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = HI3521_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = HI3521_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = HI3521_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
				  /* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = BRD_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BRD_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = BRD_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
					  /* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = RF_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RF_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RF_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ENCCODER_AB_TIM_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ENCCODER_AB_TIM_PreemptionPriority;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ENCCODER_AB_TIM_SubPriority;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
				//中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = ENCCODER_Z_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ENCCODER_Z_PreemptionPriority;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ENCCODER_Z_SubPriority;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GENERAL_TIM_PreemptionPriority;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = GENERAL_TIM_SubPriority;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

