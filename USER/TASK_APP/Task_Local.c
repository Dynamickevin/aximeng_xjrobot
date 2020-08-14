#include "includes.h"



//LED1хннЯ
void Task_Local(void *pdata)
{	  
	while(1)
	{
		LED1(LED_ON);
		delay_ms(1000);
		LED1(LED_OFF);
		delay_ms(1000);
		//printf("led2");
	};
}
