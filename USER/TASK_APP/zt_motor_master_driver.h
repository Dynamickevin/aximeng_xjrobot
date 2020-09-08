//////////////////////////////////////////////////////////////////////////
//zt_motor_master_driver.h 
//������ �������
//�ײ��������� �ṩ�����ʼ�����ٶȿ��ƣ�λ�ÿ��ƽӿ�
//�����ڲ���һ���Զ����ٶȿ��Ƹ��º������ú�����ÿ��tickʱ������һ��
//////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"

#ifndef _ZT_MOTOR_MASTER_DRIVER_H_
#define _ZT_MOTOR_MASTER_DRIVER_H_

//����
/*******************************************************/
#define Mst_MOTOR1_DIR1_PIN          			GPIO_Pin_2              
#define Mst_MOTOR1_DIR1_GPIO_PORT    			GPIOB                      
#define Mst_MOTOR1_DIR1_GPIO_CLK      		RCC_AHB1Periph_GPIOB

#define Mst_MOTOR1_DIR2_PIN          			GPIO_Pin_12              
#define Mst_MOTOR1_DIR2_GPIO_PORT    			GPIOA                      
#define Mst_MOTOR1_DIR2_GPIO_CLK      		RCC_AHB1Periph_GPIOA

#define Mst_MOTOR1_OCPWM_PIN          	GPIO_Pin_14             
#define Mst_MOTOR1_OCPWM_GPIO_PORT    	GPIOB                     
#define Mst_MOTOR1_OCPWM_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define Mst_MOTOR1_OCPWM_PINSOURCE			GPIO_PinSource14
#define Mst_MOTOR1_OCPWM_AF							GPIO_AF_TIM1

#define	Mst_MOTOR1_TIM           				TIM1
#define Mst_MOTOR1_TIM_CLK       				RCC_APB2Periph_TIM1

#define Mst_MOTOR1_TIM_OC_INIT					TIM_OC2Init

#define	Mst_MOTOR1_TIM_Period						100			//Ƶ�� = ϵͳƵ�ʣ�168M��/MOTOR1_TIM_PSC/MOTOR1_TIM_Period = 2KHZ   �ֱ���ΪMOTOR1_TIM_Period
#define	Mst_MOTOR1_TIM_PSC							840
#define	Mst_MOTOR1_SetCompare						TIM_SetCompare2

//����������Ϊ �͵�ƽʹ�� �ߵ�ƽ�ر�
//->BSRR to set io=1  ->BRR to set io=0
#define SET_MASTER_MOTOR_CLOSE() GpioSetH(GPIO_CTL_DIR_M1) ; GpioSetH(GPIO_CTL_DIR_M2);SET_MASTER_MOTOR_PWM(0)

#define SET_MASTER_MOTOR_PWM  bsp_Master_motor1_Set_Speed

#define SET_MASTER_MOTOR_ZZ()   GpioSetH(GPIO_CTL_DIR_M1) ; GpioSetL(GPIO_CTL_DIR_M2)
#define SET_MASTER_MOTOR_FZ()   GpioSetL(GPIO_CTL_DIR_M1) ; GpioSetH(GPIO_CTL_DIR_M2)

#define GPIO_CTL_DIR_M1 	 	 GPIOB,GPIO_Pin_2	  	//�����ַ���1
#define GPIO_CTL_DIR_M2 	 	 GPIOA,GPIO_Pin_12	  //�����ַ���2



// �����motor���Ƶ��ú���
/*********************************************************/
			
void master_motor1_GPIO_TIM_Init(void);		//�����������ʼ��
	
void bsp_Master_Motor1_Config(void);

void bsp_Master_Motor1_GPIO_Init(void);

void bsp_Master_motor1_Set_Speed(u16 NewSpeed);	


#define  GET_SLAVE_WHEEL_CODE()  bsp_Enccoder_AB_GET_Cnt()
#define  SLAVE_WHEEL_CODE_SCALE         2400		//���̻������ϵ��


/************************************************* 
*Function:		zt_motor_master_driver_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  �����ֵ����ʼ��
*************************************************/
void zt_motor_master_driver_init(void);

/*************************************************
*Input:			
*OUTPUT:		void
*Return:		ʵ�����õ��ٶ�
*DESCRIPTION:  ���� �����ֵ�����ٶ�
*************************************************/
s16 zt_motor_master_driver_set_speed(s16 speed,u16 code_run);

/************************************************* 
*Function:		zt_motor_master_driver_set_accl
*Input:			
*OUTPUT:		void
*Return:		ʵ�����õ� ���ٶ�
*DESCRIPTION:  ���� �����ֵ���ļ��ٶ�  ���ٶ�ֵΪ���Ƶ���ٶȱ仯ʱ��
*           ��������θ���ʱ���仯���ٶ��� �ٶ�ֵΪ 0.00��100.00 
*           Ϊ�˿��� �ͼ��㾫�� ���ٶ�ֵ ��λΪ0.01
*************************************************/
u16 zt_motor_master_driver_set_accl(u16 accl);


//////////////////////////////////////////////////////////////////////////
//��λ���� ��Ҫ�����ֿ��Ʋ��� �� float ת��Ϊ int ����
//����ٶȣ�������ֵ ���ڲ����ƶ��� int����

typedef struct
{
    u8      cur_dir;    //��ǰ�ķ���  ����ķ���0 ��ʾ����  �������Ƿ�ת
    u8      set_dir;    //���õķ���
    u16     n_accl;     //��ǰ�ļ��ٶ�
    u16     n_del_accl;     //��ǰ�ļ��ٶ�
    u16     set_speed;    //���õ��ٶ� Ŀ���ٶ�
    u16     cur_speed;    //��ǰ���ٶ� ��ֵΪ �ٶȵ�100�� ������Щβ��
    u16     left_code;    //���Ƶ���˶� ʣ�µ������� ���ﵽ��Щ������ʱ����Ҫ���ֹͣ
    
    s16   real_out_speed; 

    //limit_speed ��ͬ������� ���ٲ�ͬ ��ѹ��̫С ���� ̫��ʱ ����Ҫ��Щ
    //�������Ҫ�������٣���Ҫ��������ֵΪ 101*100
    u16     limit_speed ;  
    u16     limit_speed_time ; //���ٳ���ʱ��

    u16     ys_running_time;  //2017-04-06 ��ˢ��������ڼ���������Ƿ��쳣������������Ҫֹͣ���һ��ʱ��
    u16     ys_stop_in_running_time; //���й����� ֹͣ��ʱ�䳤��
    u16     ys_need_stop_time_err; //��ˢ����쳣ʱ����Ҫ��Ϣ��ʱ��
    
    bool	motor_master_wheel_slippage_state;	//�����ִ�״̬��־

}MotorDriverCtrlType;

extern MotorDriverCtrlType gMstMt;

/************************************************* 
*Return:	void
*DESCRIPTION:  �������ٶ�ʵ�ʿ����ٶȷ��������¹���
*************************************************/
void zt_motor_master_driver_update(void);


int32_t bsp_Enccoder_AB_GET_Cnt(void);


/*************************************************
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  �ж��������Ƿ��
*************************************************/
void zt_motor_master_wheel_slippage(void);


#endif
