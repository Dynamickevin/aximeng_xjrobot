#include  "board.h"





u8    RobotError;						//错误	

float Weight;							//压力传感器重量
u16 cnt;
u32 AB_Cnt;								//编码器计数
GPS_Info_Typedef GPS_Info;				//GPS信息
AM2320_Data_TypeDef AM2320_Data;	//AM2320 温度值	湿度值
extern int16_t Ntc_temp;					//HI3521_NTC 温度值
extern u32 Time100ms;


typedef struct _PACKED{
//	u8 error;
	BRD_Info_Typedef BRD_Info; 
	AM2320_Data_TypeDef AM2320_Data;
	float Weight;
	u32 AB_Cnt;
	GPS_Info_Typedef GPS_Info;
}STM_Robot_Info_Typedef;



STM_Robot_Info_Typedef Robot_Info;

void bsp_Board_Init(void)
{
	//NVIC_Configuration();	
	//delay_init();
	bsp_Led_Init();   
	//bsp_Pressure_Sensor_Init();
	//bsp_HI3521_GPIO_Init();	
	bsp_Video_Trans_PWR_Init();
	bsp_Limit_SW_Init();
	//bsp_motor_Init();
	//bsp_enccoder_Init();
	//GENERAL_TIM_Configuration();
	//bsp_BRD_Init();
	//bsp_GPS_Init(GPS_TTL);
	bsp_GPS_cmd(ENABLE);
	//bsp_RF_Init();
	//bsp_ADC_Init();
}
	
/*
int Robot_Demo(void)
{
//	u8 Buf[64];
	int16_t Speed = 5000;
	delay_ms(100);
//	while(bsp_RF_Send_Config_Cmd(0xff,0xff,0x17))delay_ms(500);;
	delay_ms(1000);
	 Motor1_Ctrl(0);
	 Motor2_Ctrl(0);
	while(1)
	{	
		

		delay_ms(2000);
		Motor2_Ctrl(-5000);
		delay_ms(2000);
		Motor2_Ctrl(0);
		delay_ms(2000);
		Motor2_Ctrl(5000);
		delay_ms(2000);
		Motor2_Ctrl(0);
		
//		Get_Info();
//		//STM_TX_Robot_Info();
//		bsp_RF_UART_Parse();
	}
	//return 0;
}



void Get_Info(void)
{
	GPS_Info_Typedef New_Gps_Info;   
	AM2320_Data_TypeDef Nes_data;
	Weight = bsp_Pressure_Sensor_Get_Weight();
	AB_Cnt = bsp_Enccoder_AB_GET_Cnt();
	if(bsp_GPS_Get_Info(&New_Gps_Info) == 0 && (New_Gps_Info.Status == 1 || New_Gps_Info.Status == 2)){
		memcpy(&GPS_Info,&New_Gps_Info,sizeof(New_Gps_Info));
	}
	if(bsp_AM2320_Read_Datas(&Nes_data) == 0){
		memcpy(&AM2320_Data,&Nes_data,sizeof(Nes_data));
	}
}


void STM_TX_Robot_Info(void)
{
//	u8 buf[512];
//	u32 Datalen;
	Get_Info();
	Robot_Info.AB_Cnt = AB_Cnt;
	Robot_Info.AM2320_Data.humi = AM2320_Data.humi;
	Robot_Info.AM2320_Data.temp = AM2320_Data.temp;
	memcpy(&Robot_Info.BRD_Info,&BRD_Info,sizeof(BRD_Info));
	//Robot_Info.error = RobotError;
	Robot_Info.Weight = Weight;
	memcpy(&Robot_Info.GPS_Info,&GPS_Info,sizeof(GPS_Info));
	bsp_RF_Send_Cmd(RF_GET_STATUS,sizeof(Robot_Info),(u8 *)(&Robot_Info));	
}

*/












/**************************************************END LINE**********************************************/
