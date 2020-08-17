#include  "board.h"


extern u32 Time100ms;



void bsp_Board_Init(void)
{
	//NVIC_Configuration();	
	//delay_init();
	bsp_Led_Init();   
	bsp_Pressure_Sensor_Init();
	//bsp_HI3521_GPIO_Init();	
	//bsp_Video_Trans_PWR_Init();
	bsp_Limit_SW_Init();
	//bsp_motor_Init();
	bsp_enccoder_Init();
	//GENERAL_TIM_Configuration();
	//bsp_BRD_Init();
	//bsp_GPS_Init(GPS_TTL);
	//bsp_GPS_cmd(ENABLE);
	//bsp_RF_Init();
	bsp_ADC_Init();
}
	













/**************************************************END LINE**********************************************/
