

#include  "board.h"


GPS_Info_Typedef GPS_Info;				//GPS��Ϣ

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
	bsp_enccoder_Init();							//�Ӷ������ӱ�����
	
	bsp_ADC_Init();
	AM2320_and_iic_init();	//��ʪ�ȴ�����ģ���ʼ��
	bsp_Lora_init();        //����1������LORAģ�飬��������ͨѶ
	bsp_BRD_Init();					//����2���������Դ��ͨ��
	bsp_HI3521_Init();	    //����3�������뺣˼оƬLINUXϵͳͨѶ
	bsp_Debug_Init();				//����4�����ڵ�����Ϣ��ӡ��� 
	bsp_GPS_Init(GPS_RS232);//����5�����ڽ���GPS��Ϣ
	bsp_GPS_cmd(ENABLE);
	//UART6_Config();       //����6��Ԥ�������ڵ��485����
	
	zt_motor_slave_driver_init();     //�Ӷ��ֵ��  	�����PWM��ʼ�� 
  zt_motor_master_driver_init();    //�����ֵ��  	�����PWM��ʼ��
	
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
