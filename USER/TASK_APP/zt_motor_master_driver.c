
#include <includes.h> 


MotorDriverCtrlType gMstMt;

//主电机定时器初始化
static void bsp_Master_Motor1_Config(void);
//主电机I/O口初始化
static void bsp_Master_Motor1_GPIO_Init(void);
//主电机速度设置
static void bsp_Master_motor1_Set_Speed(u16 NewSpeed);	


/*
  * @brief  主电机MOTOR初始化，包含I/O口和定时器
  * @param  无
  * @retval 无
*/
void master_motor1_GPIO_TIM_Init(void)
{
	bsp_Master_Motor1_GPIO_Init();
	bsp_Master_Motor1_Config();
	
}

/*
**主电机控制引脚I/O口初始化函数
*/
static void bsp_Master_Motor1_GPIO_Init(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (Mst_MOTOR1_DIR1_GPIO_CLK|Mst_MOTOR1_DIR2_GPIO_CLK|Mst_MOTOR1_OCPWM_GPIO_CLK, ENABLE); 					   
	//方向1、2引脚初始化为高电平
	GPIO_SetBits(Mst_MOTOR1_DIR1_GPIO_PORT,Mst_MOTOR1_DIR1_PIN);
	GPIO_SetBits(Mst_MOTOR1_DIR2_GPIO_PORT,Mst_MOTOR1_DIR2_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = Mst_MOTOR1_DIR1_PIN;		
	GPIO_Init(Mst_MOTOR1_DIR1_GPIO_PORT, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = Mst_MOTOR1_DIR2_PIN;		
	GPIO_Init(Mst_MOTOR1_DIR2_GPIO_PORT, &GPIO_InitStructure);	

	/* PB14 定时器通道引脚复用 */
	GPIO_PinAFConfig(Mst_MOTOR1_OCPWM_GPIO_PORT,Mst_MOTOR1_OCPWM_PINSOURCE,Mst_MOTOR1_OCPWM_AF);
	
	
	/* 定时器通道引脚配置 */															   
	GPIO_InitStructure.GPIO_Pin = Mst_MOTOR1_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(Mst_MOTOR1_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 */
static void bsp_Master_Motor1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
	RCC_APB2PeriphClockCmd(Mst_MOTOR1_TIM_CLK, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = Mst_MOTOR1_TIM_Period-1;       

	TIM_TimeBaseStructure.TIM_Prescaler = Mst_MOTOR1_TIM_PSC-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;

	TIM_TimeBaseInit(Mst_MOTOR1_TIM, &TIM_TimeBaseStructure);
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //配置为PWM模式2
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  	  //当定时器计数值大于CCR1_Val时为高电平
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable ;
//	TIM_OCInitStructure.TIM_OutputNState    = TIM_OutputNState_Disable;
//  TIM_OCInitStructure.TIM_OCNIdleState    = TIM_OCNIdleState_Reset;
//  TIM_OCInitStructure.TIM_OCIdleState     = TIM_OCIdleState_Reset ;
	
	Mst_MOTOR1_TIM_OC_INIT(Mst_MOTOR1_TIM, &TIM_OCInitStructure);	 //使能通道2

	TIM_OC2PreloadConfig(Mst_MOTOR1_TIM,TIM_OCPreload_Enable); //TIM_1---->通道2[PB14]

	// 使能定时器
	TIM_Cmd(Mst_MOTOR1_TIM, ENABLE);	
	TIM_CtrlPWMOutputs(Mst_MOTOR1_TIM,ENABLE);
}

/*
  * @brief  主电机1 速度设置
  * @param  10~90;  	
  * @retval 无
*/
void bsp_Master_motor1_Set_Speed(u16 NewSpeed)
{
	if(NewSpeed > Mst_MOTOR1_TIM_Period*0.9)NewSpeed = Mst_MOTOR1_TIM_Period*0.9;
	Mst_MOTOR1_SetCompare(Mst_MOTOR1_TIM,NewSpeed);
	
}


//////////////////////////////////////////////////////////////////////////
//电机速度与脉冲速度   设置电机速度为10 脉冲速度=0.5S->50=100/S  脉冲速度=电机速度*10
//按照上面的比例关系，在某个加速度，速度下，若目标速度为0，到速度为0时，脉冲数为：

//////////////////////////////////////////////////////////////////////////
/************************************************* 
*Function:		zt_motor_master_driver_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  主动轮电机初始化
*************************************************/
void zt_motor_master_driver_init(void)
{
    //PB14 PB15 主动轮 电机I/O口和定时器初始化 
		master_motor1_GPIO_TIM_Init(); 
		SET_MT_BREAK_CLOSE;					//抱闸关闭
    SET_MASTER_MOTOR_CLOSE();  	//设置电机当前速度为 0
		  			
    //编码器值 进行初始化
    //TimerCode_DefaultFunction_Init(2);  //编码器 数据采集初始化 主动轮 TIM2
    memset( &gMstMt , 0 , sizeof(gMstMt) );
		memset( &gSpeedAnaly_Mst, 0 , sizeof(SpeedAnalyByCode) );
	
    gMstMt.limit_speed  = 101*100 ;
    SET_MASTER_MOTOR_CLOSE();		//设置电机当前速度为 0
}

/************************************************* 
*Function zt_motor_master_driver_set_speed
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  设置 主动轮电机的速度
*************************************************/
s16 zt_motor_master_driver_set_speed(s16 speed,u16 code_run)
{
    u8 bFanxiang = 0;
    if ( speed < 0 )
    {
        speed = - speed;
        bFanxiang = 1;
//			nprintf(ID_DEBUG,speed,0,DEC);
//			//debug_sprintf(ID_DEBUG,"1111");
//			debug_sprintf(ID_DEBUG,"\r\n");
    }
    else if( speed > 0 )
		{
       speed = speed;
       bFanxiang = 0;
//			nprintf(ID_DEBUG,speed,0,DEC);
//			//debug_sprintf(ID_DEBUG,"1111");
//			debug_sprintf(ID_DEBUG,"\r\n");
    }
		else
		{
			  SET_MASTER_MOTOR_CLOSE();
		}
		
    if ( speed >= 90 )
    {
        speed = 90;
    }
    else if ( speed < 10 )
    {
        speed = 0;
    }
    gMstMt.left_code = code_run ;

    gMstMt.set_dir   = bFanxiang ;
    gMstMt.set_speed = speed * 100 ;  //速度需要乘以100
		//nprintf(ID_DEBUG,gMstMt.set_speed,0,DEC);
    //SET_MASTER_MOTOR_PWM(speed);
		//SET_MASTER_MOTOR_FZ();
		
    return (bFanxiang)?(-speed):(speed);
}

#if 0
/************************************************* 
*Return:	void
*DESCRIPTION:  主动轮速度实际控制速度分析及更新过程
*************************************************/
void zt_motor_master_driver_update(void)
{
    static u16 real_mst_mt_set_speed = 0;
    static u16 real_mst_mt_set_accl  = 0;
    static u16 real_mst_mt_set_del_accl  = 0;

    //根据设置的速度 和 限速，分析实际需要的设置速度 和 加速度，减速度值
    //然后更新电机状态
    if ( gMstMt.set_speed >= gMstMt.limit_speed )
    {
        real_mst_mt_set_speed       = gMstMt.limit_speed;
        real_mst_mt_set_accl        = 1 ;      //加速度减慢
        real_mst_mt_set_del_accl    = 220 ;    //减速度加快
    }
    else
    {
        real_mst_mt_set_speed       = gMstMt.set_speed;
        real_mst_mt_set_accl        = gMstMt.n_accl ;    
        real_mst_mt_set_del_accl    = gMstMt.n_del_accl ;
    }

    //////主动轮运行过程中 需要判断是否卡死异常，如果是，需要暂停一段时间后再继续
    if (  1300 < gMstMt.cur_speed  ) 
    {
        gMstMt.ys_running_time++;
        if( (gSpeedAnaly_Mst.speed<5) && (gSpeedAnaly_Mst.speed>-5) )
        {
            gMstMt.ys_stop_in_running_time++;
            if ( gMstMt.ys_stop_in_running_time > 800 ) //卡死一段时间了 需要先停止下，在启动
            {
                gMstMt.ys_stop_in_running_time = 0;
                gMstMt.ys_need_stop_time_err = 100;
                real_mst_mt_set_speed = 0;
                gMstMt.cur_speed = 0;
                gMstMt.cur_dir = gMstMt.set_dir;
                SET_MASTER_MOTOR_CLOSE();
                SET_MT_BREAK_CLOSE;
            }
        }
        else{
            if(gMstMt.ys_stop_in_running_time){
                gMstMt.ys_stop_in_running_time--;
            }
        }
    }
    else if ( gMstMt.ys_need_stop_time_err )
    {
        gMstMt.ys_need_stop_time_err--;
        real_mst_mt_set_speed = 0;
        gMstMt.cur_speed = 0;
        gMstMt.cur_dir = gMstMt.set_dir;
    }
    else  //主动轮已经停止
    {
        gMstMt.ys_running_time = 0;
        gMstMt.ys_stop_in_running_time = 0;
    }


    if ( (real_mst_mt_set_speed!=gMstMt.cur_speed) || (gMstMt.set_dir!=gMstMt.cur_dir) )
    {
        if ( gMstMt.set_dir!=gMstMt.cur_dir )
        {
            if ( gMstMt.cur_speed > real_mst_mt_set_del_accl )
            {
                gMstMt.cur_speed -= real_mst_mt_set_del_accl ;
            }
            else
            {
                gMstMt.cur_speed = 0 ;
                gMstMt.cur_dir = gMstMt.set_dir ;
            }
        }
        else if ( real_mst_mt_set_speed > gMstMt.cur_speed )  //需要加速
        {
            gMstMt.cur_speed += real_mst_mt_set_accl ;
            if( gMstMt.cur_speed > real_mst_mt_set_speed )
            {
                gMstMt.cur_speed = real_mst_mt_set_speed ;
            }
        }
        else  //real_mst_mt_set_speed < gMstMt.cur_speed  需要减速 最多减速到设置的值
        {
            if ( (real_mst_mt_set_speed+real_mst_mt_set_del_accl) >= gMstMt.cur_speed )
            {
                gMstMt.cur_speed = real_mst_mt_set_speed ;
            }
            else{
                gMstMt.cur_speed -= real_mst_mt_set_del_accl ;
            }
        }
        
#if 1 //ifdef SET_MASTER_MOTOR_CLOSE
        if ( gMstMt.cur_speed <= 10 )
        {
            SET_MASTER_MOTOR_CLOSE();
            SET_MT_BREAK_CLOSE;  //控制抱闸关闭
        }
        if( gMstMt.cur_speed < 1100 )
        {
            SET_MASTER_MOTOR_CLOSE();
            //SET_MT_BREAK_OPEN;
        }
        else if ( gMstMt.cur_dir )
        {
            SET_MASTER_MOTOR_FZ() ;
            SET_MT_BREAK_OPEN;
        } 
        else
        {
            SET_MASTER_MOTOR_ZZ() ;
            SET_MT_BREAK_OPEN;
        }
#endif
        SET_MASTER_MOTOR_PWM( gMstMt.cur_speed/100 ); 
    }
}

#else  //2017-11-29

/************************************************* 
*Return:	void
*DESCRIPTION:  主动轮 速度限制时间更新 速度限制只需要持续一定时间，
*       时间到了，不再进行速度限制
*************************************************/
static void __inline DoLimitMstMtSpeed_Stop(void) 
{
    if ( gMstMt.limit_speed_time )
    {
        gMstMt.limit_speed_time--;
    }
    else
		{
        gMstMt.limit_speed = 101*100;
    }
}


/************************************************* 
*static 静态全局变量 作用范围该文件
*Return:	void
*DESCRIPTION:  主动轮速度实际控制速度分析及更新过程
*************************************************/
void zt_motor_master_driver_update(void)
{
    static u16 real_mst_mt_set_speed = 0;
    static u16 real_mst_mt_set_accl  = 0;
    static u16 real_mst_mt_set_del_accl  = 0;
    static bool bMtCheckStop = false; 

    //电机采集的状态值，判断当前电机实际是否已经停止
    if( ( ( gMstMt.cur_dir) && (gMtDisControlCheck.mstMtBackSpeed.lvVal>-4) )||
        ( (!gMstMt.cur_dir) && (gMtDisControlCheck.mstMtBackSpeed.lvVal< 4) ) )
    { //实际速度为0
        bMtCheckStop = true;
    }
    else
		{
        bMtCheckStop = false;
    }
    
    //根据设置的速度 和 限速，分析实际需要的设置速度 和 加速度，减速度值
    //然后更新电机状态
    DoLimitMstMtSpeed_Stop();
		
    if ( gMstMt.set_speed >= gMstMt.limit_speed )
    {   
        real_mst_mt_set_speed       = gMstMt.limit_speed;
        real_mst_mt_set_accl        = gSlvMtCfg.mstAddAccl/2 ;      //加速度减慢
        real_mst_mt_set_del_accl    = gSlvMtCfg.mstDelAccl*6 ;    //减速度加快
        
        //限制速度为0
        if( gMstMt.limit_speed == 0 )
				{
            bMtCheckStop = true;  //强制识别为停止状态
        }
    }
    else
    {  
        real_mst_mt_set_speed       = gMstMt.set_speed;
        real_mst_mt_set_accl        = gSlvMtCfg.mstAddAccl ;    
        real_mst_mt_set_del_accl    = gSlvMtCfg.mstDelAccl ;
    }
    
    //如果当前检测到主电机失控，需要强制关闭主电机一段时间
    if( gMtDisControlCheck.mstNeedStopCnt > 0 )
		{
      gMtDisControlCheck.mstNeedStopCnt--;
      real_mst_mt_set_speed    = 0 ;
      real_mst_mt_set_del_accl = gSlvMtCfg.mstDelAccl*100 ;    //减速度加快
			debug_sprintf(ID_DEBUG,"Master Motor out of control");
    }  

    //设定速度方向 与当前实际输出 速度和方向不同；需要进行切换处理
    //先判断控制方向 与 当前电机运动方向是否相同
    if( gMstMt.set_dir != gMstMt.cur_dir )  //需要切换方向的处理过程
    {   
        if( bMtCheckStop )
				{ //实际速度为0
            gMstMt.cur_speed = 11*100;
            gMstMt.cur_dir = gMstMt.set_dir;
        }
        else
				{ 	//如果当前有一个设定速度，按照加速度值进行减速
            if( gMstMt.cur_speed > real_mst_mt_set_del_accl )
						{
                gMstMt.cur_speed -= real_mst_mt_set_del_accl;
            }
            else
						{
                gMstMt.cur_speed = 0;
            }
        }
    }
    else if( real_mst_mt_set_speed != gMstMt.cur_speed )
		{ 	//速度有变化
				if ( real_mst_mt_set_speed > gMstMt.cur_speed )   //需要加速
        {
            gMstMt.cur_speed += real_mst_mt_set_accl ;
            if( gMstMt.cur_speed > real_mst_mt_set_speed )
            {
                gMstMt.cur_speed = real_mst_mt_set_speed ;
            }
            if( gMstMt.cur_speed < 11*100 )
						{
                gMstMt.cur_speed = 11*100 ;
            }
        }
        else  //real_mst_mt_set_speed < gMstMt.cur_speed  需要减速 最多减速到设置的值
        {
            if ( (real_mst_mt_set_speed+real_mst_mt_set_del_accl) >= gMstMt.cur_speed )
            {
                gMstMt.cur_speed = real_mst_mt_set_speed ;
            }
            else
						{
                gMstMt.cur_speed -= real_mst_mt_set_del_accl ;
            }
        }
    }
    
    if( (gMstMt.cur_speed > (10*100-1)) )
		{
       //Send_Power_Board_Master_State(ENABLE);
			SET_MT_BREAK_OPEN ;
    }
    else if( (!bMtCheckStop)  )
		{
        if ( gMstMt.limit_speed_time )  //在桥上，立即停止
        {
					//Send_Power_Board_Master_State(DISABLE);
					SET_MT_BREAK_CLOSE ;
        }
        else
				{ //不在桥上，等待停止
					//Send_Power_Board_Master_State(ENABLE);
          SET_MT_BREAK_OPEN ;
        }
    }
    else
		{
      //Send_Power_Board_Master_State(DISABLE);
			//SET_MT_BREAK_CLOSE ;
    }
    
    real_mst_mt_set_speed = gMstMt.cur_speed / 100;
    SET_MASTER_MOTOR_PWM( real_mst_mt_set_speed );  //速度值
		
    gMstMt.real_out_speed = (gMstMt.cur_dir)? (-real_mst_mt_set_speed):(real_mst_mt_set_speed) ;
    
    if( gMstMt.cur_speed < 9*100)
		{
        SET_MASTER_MOTOR_CLOSE();
    }
    else if ( gMstMt.cur_dir )
    {
        SET_MASTER_MOTOR_FZ() ;
    } 
    else
    {
        SET_MASTER_MOTOR_ZZ() ;
    }
}
#endif

/************************************************* 
*Function:		zt_motor_master_driver_set_accl
*Input:			
*OUTPUT:		u16 accl
*Return:		实际设置的 加速度
*DESCRIPTION:  设置 主动轮电机的加速度  加速度值为控制电机速度变化时，
*           电机在两次更新时，变化的速度量 速度值为 0.00～100.00 
*           为了控制 和计算精度 加速度值 单位为0.01
*************************************************/
u16 zt_motor_master_driver_set_accl(u16 accl)
{
    if ( accl < 1 )
    {
        accl = 1;
    }
    else if ( accl > 12000 )
    {
        accl = 12000 ;
    }
    gMstMt.n_accl = accl;
    return accl;
}


/************************************************* 
*
*Return:	void
*DESCRIPTION:  主动轮速度打滑状态检测函数
*************************************************/
void zt_motor_master_wheel_slippage(void)
{
	signed short befor_master_position;
	signed short after_master_position;
	//char Master_Motor_State[8];  //主动轮状态：N:normal,正常状态，    	  	S:slip, 打滑状态
	gMstMt.motor_master_wheel_slippage_state = false;
	//Master_Motor_State[0] = 'N';

	if(gMstMt.set_speed != 0)
	{
		befor_master_position = (signed short)(GET_SLAVE_WHEEL_CODE()/SLAVE_WHEEL_CODE_SCALE);
		OSTimeDly(OS_TICKS_PER_SEC);
		after_master_position = (signed short)(GET_SLAVE_WHEEL_CODE()/SLAVE_WHEEL_CODE_SCALE);
		if(befor_master_position == after_master_position)
		{

			gMstMt.motor_master_wheel_slippage_state = true;
			//Master_Motor_State[0] = 'S';
			stprintf(ID_DEBUG,"motor master wheel is slip \r\n");

		}
	}

}





