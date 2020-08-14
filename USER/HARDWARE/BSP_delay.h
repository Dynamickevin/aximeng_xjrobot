#ifndef __DELAY_H
#define __DELAY_H 			   
#include "includes.h"	  
//////////////////////////////////////////////////////////////////////////////////  
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//修改日期:2020-07-13
//版本：V0.1
//********************************************************************************





void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























