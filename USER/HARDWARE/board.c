#include  "board.h"


extern u32 Time100ms;



void bsp_Board_Init(void)
{
	
	//NVIC_Configuration();	
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
	
	wheel_encode_Init();		//从动轮轮子编码器
	//AM2320_and_iic_init();	//温湿度传感器模块初始化
	USART1_Config();        //串口1，用于LORA模块，本地无线通讯
	USART2_Config();        //串口2，用于与电源板通信
	USART3_Config();        //串口3，用于与海思芯片LINUX系统通讯
	UART4_Config();         //串口4，用于与GPS模块TTL通讯
	UART5_Config();         //串口5，用于与GPS模块RS232通讯
	
	//TimerCode_DefaultFunction_Init(4);  //从动轮轮子编码器 定时器、数据采集初始化
	MST_PWM_Init_Tim1_CH2();			//主动轮电机PWM波初始化配置函数
	SLV_PWM_Init_Tim8_CH3();				//从动轮电机PWM波初始化配置函数
	SetPwm_Tim1_CH2(0); 
	SetPwm_Tim8_CH3(0);
	zt_motor_slave_driver_init();     //从动轮电机  		   初始化 
  zt_motor_master_driver_init();    //主动轮电机  		   初始化
	
}
	













/**************************************************END LINE**********************************************/
