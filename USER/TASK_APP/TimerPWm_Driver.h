

#ifndef __TIMER_PWM_DRIVER_H
#define __TIMER_PWM_DRIVER_H

#define MAX_HARD_TIMER_CNT    8
extern TIM_TypeDef* const gHardTimerList[MAX_HARD_TIMER_CNT+1];
static __inline TIM_TypeDef* GetTimerByID(u8 id)
{
    return gHardTimerList[id];
}

//////////////////////////////////////////////////////////////////////////
//定时器 编码器采集功能驱动  TIM1  ~  TIM8   1 to 8
void TimerCode_DefaultFunction_Init(u32 timer_ID);

//CNT下面宏定义 传入参数为 TIM2 / TIM3 / TIM4 ...
#define GetTimerCodeVal(timer_P)  ( (timer_P)->CNT )


//////////////////////////////////////////////////////////////////////////
//定时器  PWM输出控制初始化 这部分功能需要用户根据实际情况自定义设计
void MST_PWM_Init_Tim1_CH2(void);   //主动轮PWM PB14
void SLV_PWM_Init_Tim8_CH3(void);		//从动轮PWM PB15

//使用下面的 函数，可以增加代码执行效率，不需要执行参数是否正确的判断
//设置 从动轮 速度
static __inline void SetPwm_Tim1_CH2(uint16_t Compare2)
{
    //TIM_SetCompare2( TIM1 , Compare2 );
    TIM1->CCR2 = Compare2;
}
//设置 主动轮 速度
static __inline void SetPwm_Tim8_CH3(uint16_t Compare3)
{
    //TIM_SetCompare3( TIM8 , Compare3 );
    TIM8->CCR3 = Compare3;
}




#endif
