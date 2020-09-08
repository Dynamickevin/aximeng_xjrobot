//////////////////////////////////////////////////////////////////////////
//zt_motor_master_driver.h 
//主动轮 电机控制
//底层驱动级别 提供电机初始化，速度控制，位置控制接口
//驱动内部有一个自动的速度控制更新函数，该函数在每次tick时被调用一次
//////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"

#ifndef _ZT_MOTOR_MASTER_DRIVER_H_
#define _ZT_MOTOR_MASTER_DRIVER_H_

//定义
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

#define	Mst_MOTOR1_TIM_Period						100			//频率 = 系统频率（168M）/MOTOR1_TIM_PSC/MOTOR1_TIM_Period = 2KHZ   分辨率为MOTOR1_TIM_Period
#define	Mst_MOTOR1_TIM_PSC							840
#define	Mst_MOTOR1_SetCompare						TIM_SetCompare2

//驱动器设置为 低电平使能 高电平关闭
//->BSRR to set io=1  ->BRR to set io=0
#define SET_MASTER_MOTOR_CLOSE() GpioSetH(GPIO_CTL_DIR_M1) ; GpioSetH(GPIO_CTL_DIR_M2);SET_MASTER_MOTOR_PWM(0)

#define SET_MASTER_MOTOR_PWM  bsp_Master_motor1_Set_Speed

#define SET_MASTER_MOTOR_ZZ()   GpioSetH(GPIO_CTL_DIR_M1) ; GpioSetL(GPIO_CTL_DIR_M2)
#define SET_MASTER_MOTOR_FZ()   GpioSetL(GPIO_CTL_DIR_M1) ; GpioSetH(GPIO_CTL_DIR_M2)

#define GPIO_CTL_DIR_M1 	 	 GPIOB,GPIO_Pin_2	  	//主动轮方向1
#define GPIO_CTL_DIR_M2 	 	 GPIOA,GPIO_Pin_12	  //主动轮方向2



// 主电机motor控制调用函数
/*********************************************************/
			
void master_motor1_GPIO_TIM_Init(void);		//主电机驱动初始化
	
void bsp_Master_Motor1_Config(void);

void bsp_Master_Motor1_GPIO_Init(void);

void bsp_Master_motor1_Set_Speed(u16 NewSpeed);	


#define  GET_SLAVE_WHEEL_CODE()  bsp_Enccoder_AB_GET_Cnt()
#define  SLAVE_WHEEL_CODE_SCALE         2400		//码盘换算比例系数


/************************************************* 
*Function:		zt_motor_master_driver_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  主动轮电机初始化
*************************************************/
void zt_motor_master_driver_init(void);

/*************************************************
*Input:			
*OUTPUT:		void
*Return:		实际设置的速度
*DESCRIPTION:  设置 主动轮电机的速度
*************************************************/
s16 zt_motor_master_driver_set_speed(s16 speed,u16 code_run);

/************************************************* 
*Function:		zt_motor_master_driver_set_accl
*Input:			
*OUTPUT:		void
*Return:		实际设置的 加速度
*DESCRIPTION:  设置 主动轮电机的加速度  加速度值为控制电机速度变化时，
*           电机在两次更新时，变化的速度量 速度值为 0.00～100.00 
*           为了控制 和计算精度 加速度值 单位为0.01
*************************************************/
u16 zt_motor_master_driver_set_accl(u16 accl);


//////////////////////////////////////////////////////////////////////////
//单位换算 需要将各种控制参数 从 float 转换为 int 类型
//电机速度，编码器值 等内部控制都是 int类型

typedef struct
{
    u8      cur_dir;    //当前的方向  电机的方向0 表示正传  其他都是反转
    u8      set_dir;    //设置的方向
    u16     n_accl;     //当前的加速度
    u16     n_del_accl;     //当前的加速度
    u16     set_speed;    //设置的速度 目标速度
    u16     cur_speed;    //当前的速度 其值为 速度的100倍 并增加些尾数
    u16     left_code;    //控制电机运动 剩下的脉冲数 当达到这些脉冲数时，需要电机停止
    
    s16   real_out_speed; 

    //limit_speed 不同的情况下 限速不同 但压力太小 或者 太大时 限速要多些
    //如果不需要进行限速，需要设置限速值为 101*100
    u16     limit_speed ;  
    u16     limit_speed_time ; //限速持续时间

    u16     ys_running_time;  //2017-04-06 有刷电机，用于检测主动轮是否异常卡死；卡死需要停止电机一段时间
    u16     ys_stop_in_running_time; //运行过程中 停止的时间长度
    u16     ys_need_stop_time_err; //有刷电机异常时，需要休息的时间
    
    bool	motor_master_wheel_slippage_state;	//主动轮打滑状态标志

}MotorDriverCtrlType;

extern MotorDriverCtrlType gMstMt;

/************************************************* 
*Return:	void
*DESCRIPTION:  主动轮速度实际控制速度分析及更新过程
*************************************************/
void zt_motor_master_driver_update(void);


int32_t bsp_Enccoder_AB_GET_Cnt(void);


/*************************************************
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  判断主动轮是否打滑
*************************************************/
void zt_motor_master_wheel_slippage(void);


#endif
