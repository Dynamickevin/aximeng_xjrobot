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
	
	wheel_encode_Init();		//�Ӷ������ӱ�����
	//AM2320_and_iic_init();	//��ʪ�ȴ�����ģ���ʼ��
	USART1_Config();        //����1������LORAģ�飬��������ͨѶ
	USART2_Config();        //����2���������Դ��ͨ��
	USART3_Config();        //����3�������뺣˼оƬLINUXϵͳͨѶ
	UART4_Config();         //����4��������GPSģ��TTLͨѶ
	UART5_Config();         //����5��������GPSģ��RS232ͨѶ
	
	//TimerCode_DefaultFunction_Init(4);  //�Ӷ������ӱ����� ��ʱ�������ݲɼ���ʼ��
	MST_PWM_Init_Tim1_CH2();			//�����ֵ��PWM����ʼ�����ú���
	SLV_PWM_Init_Tim8_CH3();				//�Ӷ��ֵ��PWM����ʼ�����ú���
	SetPwm_Tim1_CH2(0); 
	SetPwm_Tim8_CH3(0);
	zt_motor_slave_driver_init();     //�Ӷ��ֵ��  		   ��ʼ�� 
  zt_motor_master_driver_init();    //�����ֵ��  		   ��ʼ��
	
}
	













/**************************************************END LINE**********************************************/
