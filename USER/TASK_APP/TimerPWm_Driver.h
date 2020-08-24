

#ifndef __TIMER_PWM_DRIVER_H
#define __TIMER_PWM_DRIVER_H

#define MAX_HARD_TIMER_CNT    8
extern TIM_TypeDef* const gHardTimerList[MAX_HARD_TIMER_CNT+1];
static __inline TIM_TypeDef* GetTimerByID(u8 id)
{
    return gHardTimerList[id];
}

//////////////////////////////////////////////////////////////////////////
//��ʱ�� �������ɼ���������  TIM1  ~  TIM8   1 to 8
void TimerCode_DefaultFunction_Init(u32 timer_ID);

//CNT����궨�� �������Ϊ TIM2 / TIM3 / TIM4 ...
#define GetTimerCodeVal(timer_P)  ( (timer_P)->CNT )


//////////////////////////////////////////////////////////////////////////
//��ʱ��  PWM������Ƴ�ʼ�� �ⲿ�ֹ�����Ҫ�û�����ʵ������Զ������
void MST_PWM_Init_Tim1_CH2(void);   //������PWM PB14
void SLV_PWM_Init_Tim8_CH3(void);		//�Ӷ���PWM PB15

//ʹ������� �������������Ӵ���ִ��Ч�ʣ�����Ҫִ�в����Ƿ���ȷ���ж�
//���� �Ӷ��� �ٶ�
static __inline void SetPwm_Tim1_CH2(uint16_t Compare2)
{
    //TIM_SetCompare2( TIM1 , Compare2 );
    TIM1->CCR2 = Compare2;
}
//���� ������ �ٶ�
static __inline void SetPwm_Tim8_CH3(uint16_t Compare3)
{
    //TIM_SetCompare3( TIM8 , Compare3 );
    TIM8->CCR3 = Compare3;
}




#endif
