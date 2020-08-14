#include "BSP_Video_TRANS.h"
#include <math.h>
#include "string.h"

/***
	***********************************************************************
	*							BSP_Video_TRANS 电源控制
	*
	***********************************************************************
***/

/*
  * @brief  Led初始化
  * @param  无
  * @retval 无
*/
void  bsp_Video_Trans_PWR_Init(void)
{

	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd ( VEDIO_TRANS_PWR_EN_GPIO_CLK, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = VEDIO_TRANS_PWR_EN_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(VEDIO_TRANS_PWR_EN_PORT, &GPIO_InitStructure);	
	bsp_Video_Trans_PWR(DISABLE);
	
}

void bsp_Video_Trans_PWR(FunctionalState NewSta)
{
	if(NewSta == ENABLE){
		GPIO_SetBits(VEDIO_TRANS_PWR_EN_PORT,VEDIO_TRANS_PWR_EN_PIN);
	}else{	
		GPIO_ResetBits(VEDIO_TRANS_PWR_EN_PORT,VEDIO_TRANS_PWR_EN_PIN);
	}
}



