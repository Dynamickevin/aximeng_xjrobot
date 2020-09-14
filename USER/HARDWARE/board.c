

#include  "board.h"


GPS_Info_Typedef GPS_Info;				//GPS信息

typedef struct _PACKED{
//	u8 error;
	BRD_Info_Typedef BRD_Info; 
	//AM2320_Data_TypeDef AM2320_Data;
	float Weight;
	u32 AB_Cnt;
	GPS_Info_Typedef GPS_Info;
}STM_Robot_Info_Typedef;

STM_Robot_Info_Typedef Robot_Info;


void bsp_Board_Init(void)
{
	
	NVIC_Configuration();	
	bsp_Led_Init();   
	bsp_slave_cal_speed_Init();
	bsp_master_cal_speed_Init();
	bsp_Pressure_Sensor_Init();
	bsp_Video_Trans_PWR_Init();
	bsp_Limit_SW_Init();
	bsp_enccoder_Init();							//从动轮轮子编码器
	
	bsp_ADC_Init();
	AM2320_and_iic_init();	//温湿度传感器模块初始化
	bsp_Lora_init();        //串口1，用于LORA模块，本地无线通讯
	bsp_BRD_Init();					//串口2，用于与电源板通信
	bsp_HI3521_Init();	    //串口3，用于与海思芯片LINUX系统通讯
	bsp_Debug_Init();				//串口4，用于调试信息打印输出 
	bsp_GPS_Init(GPS_RS232);//串口5，用于接收GPS信息
	bsp_GPS_cmd(ENABLE);
	//UART6_Config();       //串口6，预留，用于电机485控制
	
	zt_motor_slave_driver_init();     //从动轮电机  	方向和PWM初始化 
  zt_motor_master_driver_init();    //主动轮电机  	方向和PWM初始化
	
}
	




void Get_Info(void)
{
	GPS_Info_Typedef New_Gps_Info;   
	
	if(bsp_GPS_Get_Info(&New_Gps_Info) == 0 && (New_Gps_Info.Status == 1 || New_Gps_Info.Status == 2))
	{
		memcpy(&GPS_Info,&New_Gps_Info,sizeof(New_Gps_Info));
	}
	
}


void STM_TX_Robot_Info(void)
{
//	u8 buf[512];
//	u32 Datalen;
	Get_Info();
	memcpy(&Robot_Info.BRD_Info,&BRD_Info,sizeof(BRD_Info));
	memcpy(&Robot_Info.GPS_Info,&GPS_Info,sizeof(GPS_Info));
	//bsp_RF_Send_Cmd(RF_GET_STATUS,sizeof(Robot_Info),(u8 *)(&Robot_Info));	
}












/**************************************************END LINE**********************************************/
