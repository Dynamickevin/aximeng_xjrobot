#ifndef __DELAY_H
#define __DELAY_H 			   
#include "includes.h"	  
//////////////////////////////////////////////////////////////////////////////////  
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms
//�޸�����:2020-07-13
//�汾��V0.1
//********************************************************************************





void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























