
#include <includes.h> 


MotorDriverCtrlType gMstMt;

//�������ʱ����ʼ��
static void bsp_Master_Motor1_Config(void);
//�����I/O�ڳ�ʼ��
static void bsp_Master_Motor1_GPIO_Init(void);
//������ٶ�����
static void bsp_Master_motor1_Set_Speed(u16 NewSpeed);	


/*
  * @brief  �����MOTOR��ʼ��������I/O�ںͶ�ʱ��
  * @param  ��
  * @retval ��
*/
void master_motor1_GPIO_TIM_Init(void)
{
	bsp_Master_Motor1_GPIO_Init();
	bsp_Master_Motor1_Config();
	
}

/*
**�������������I/O�ڳ�ʼ������
*/
static void bsp_Master_Motor1_GPIO_Init(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (Mst_MOTOR1_DIR1_GPIO_CLK|Mst_MOTOR1_DIR2_GPIO_CLK|Mst_MOTOR1_OCPWM_GPIO_CLK, ENABLE); 					   
	//����1��2���ų�ʼ��Ϊ�ߵ�ƽ
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

	/* PB14 ��ʱ��ͨ�����Ÿ��� */
	GPIO_PinAFConfig(Mst_MOTOR1_OCPWM_GPIO_PORT,Mst_MOTOR1_OCPWM_PINSOURCE,Mst_MOTOR1_OCPWM_AF);
	
	
	/* ��ʱ��ͨ���������� */															   
	GPIO_InitStructure.GPIO_Pin = Mst_MOTOR1_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(Mst_MOTOR1_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 */
static void bsp_Master_Motor1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
	RCC_APB2PeriphClockCmd(Mst_MOTOR1_TIM_CLK, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = Mst_MOTOR1_TIM_Period-1;       

	TIM_TimeBaseStructure.TIM_Prescaler = Mst_MOTOR1_TIM_PSC-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;

	TIM_TimeBaseInit(Mst_MOTOR1_TIM, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	    //����ΪPWMģʽ2
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  	  //����ʱ������ֵ����CCR1_ValʱΪ�ߵ�ƽ
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable ;
//	TIM_OCInitStructure.TIM_OutputNState    = TIM_OutputNState_Disable;
//  TIM_OCInitStructure.TIM_OCNIdleState    = TIM_OCNIdleState_Reset;
//  TIM_OCInitStructure.TIM_OCIdleState     = TIM_OCIdleState_Reset ;
	
	Mst_MOTOR1_TIM_OC_INIT(Mst_MOTOR1_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��2

	TIM_OC2PreloadConfig(Mst_MOTOR1_TIM,TIM_OCPreload_Enable); //TIM_1---->ͨ��2[PB14]

	// ʹ�ܶ�ʱ��
	TIM_Cmd(Mst_MOTOR1_TIM, ENABLE);	
	TIM_CtrlPWMOutputs(Mst_MOTOR1_TIM,ENABLE);
}

/*
  * @brief  �����1 �ٶ�����
  * @param  10~90;  	
  * @retval ��
*/
void bsp_Master_motor1_Set_Speed(u16 NewSpeed)
{
	if(NewSpeed > Mst_MOTOR1_TIM_Period*0.9)NewSpeed = Mst_MOTOR1_TIM_Period*0.9;
	Mst_MOTOR1_SetCompare(Mst_MOTOR1_TIM,NewSpeed);
	
}


//////////////////////////////////////////////////////////////////////////
//����ٶ��������ٶ�   ���õ���ٶ�Ϊ10 �����ٶ�=0.5S->50=100/S  �����ٶ�=����ٶ�*10
//��������ı�����ϵ����ĳ�����ٶȣ��ٶ��£���Ŀ���ٶ�Ϊ0�����ٶ�Ϊ0ʱ��������Ϊ��

//////////////////////////////////////////////////////////////////////////
/************************************************* 
*Function:		zt_motor_master_driver_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  �����ֵ����ʼ��
*************************************************/
void zt_motor_master_driver_init(void)
{
    //PB14 PB15 ������ ���I/O�ںͶ�ʱ����ʼ�� 
		master_motor1_GPIO_TIM_Init(); 
		SET_MT_BREAK_CLOSE;					//��բ�ر�
    SET_MASTER_MOTOR_CLOSE();  	//���õ����ǰ�ٶ�Ϊ 0
		  			
    //������ֵ ���г�ʼ��
    //TimerCode_DefaultFunction_Init(2);  //������ ���ݲɼ���ʼ�� ������ TIM2
    memset( &gMstMt , 0 , sizeof(gMstMt) );
		memset( &gSpeedAnaly_Mst, 0 , sizeof(SpeedAnalyByCode) );
	
    gMstMt.limit_speed  = 101*100 ;
    SET_MASTER_MOTOR_CLOSE();		//���õ����ǰ�ٶ�Ϊ 0
}

/************************************************* 
*Function zt_motor_master_driver_set_speed
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  ���� �����ֵ�����ٶ�
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
    gMstMt.set_speed = speed * 100 ;  //�ٶ���Ҫ����100
		//nprintf(ID_DEBUG,gMstMt.set_speed,0,DEC);
    //SET_MASTER_MOTOR_PWM(speed);
		//SET_MASTER_MOTOR_FZ();
		
    return (bFanxiang)?(-speed):(speed);
}

#if 0
/************************************************* 
*Return:	void
*DESCRIPTION:  �������ٶ�ʵ�ʿ����ٶȷ��������¹���
*************************************************/
void zt_motor_master_driver_update(void)
{
    static u16 real_mst_mt_set_speed = 0;
    static u16 real_mst_mt_set_accl  = 0;
    static u16 real_mst_mt_set_del_accl  = 0;

    //�������õ��ٶ� �� ���٣�����ʵ����Ҫ�������ٶ� �� ���ٶȣ����ٶ�ֵ
    //Ȼ����µ��״̬
    if ( gMstMt.set_speed >= gMstMt.limit_speed )
    {
        real_mst_mt_set_speed       = gMstMt.limit_speed;
        real_mst_mt_set_accl        = 1 ;      //���ٶȼ���
        real_mst_mt_set_del_accl    = 220 ;    //���ٶȼӿ�
    }
    else
    {
        real_mst_mt_set_speed       = gMstMt.set_speed;
        real_mst_mt_set_accl        = gMstMt.n_accl ;    
        real_mst_mt_set_del_accl    = gMstMt.n_del_accl ;
    }

    //////���������й����� ��Ҫ�ж��Ƿ����쳣������ǣ���Ҫ��ͣһ��ʱ����ټ���
    if (  1300 < gMstMt.cur_speed  ) 
    {
        gMstMt.ys_running_time++;
        if( (gSpeedAnaly_Mst.speed<5) && (gSpeedAnaly_Mst.speed>-5) )
        {
            gMstMt.ys_stop_in_running_time++;
            if ( gMstMt.ys_stop_in_running_time > 800 ) //����һ��ʱ���� ��Ҫ��ֹͣ�£�������
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
    else  //�������Ѿ�ֹͣ
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
        else if ( real_mst_mt_set_speed > gMstMt.cur_speed )  //��Ҫ����
        {
            gMstMt.cur_speed += real_mst_mt_set_accl ;
            if( gMstMt.cur_speed > real_mst_mt_set_speed )
            {
                gMstMt.cur_speed = real_mst_mt_set_speed ;
            }
        }
        else  //real_mst_mt_set_speed < gMstMt.cur_speed  ��Ҫ���� �����ٵ����õ�ֵ
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
            SET_MT_BREAK_CLOSE;  //���Ʊ�բ�ر�
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
*DESCRIPTION:  ������ �ٶ�����ʱ����� �ٶ�����ֻ��Ҫ����һ��ʱ�䣬
*       ʱ�䵽�ˣ����ٽ����ٶ�����
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
*static ��̬ȫ�ֱ��� ���÷�Χ���ļ�
*Return:	void
*DESCRIPTION:  �������ٶ�ʵ�ʿ����ٶȷ��������¹���
*************************************************/
void zt_motor_master_driver_update(void)
{
    static u16 real_mst_mt_set_speed = 0;
    static u16 real_mst_mt_set_accl  = 0;
    static u16 real_mst_mt_set_del_accl  = 0;
    static bool bMtCheckStop = false; 

    //����ɼ���״ֵ̬���жϵ�ǰ���ʵ���Ƿ��Ѿ�ֹͣ
    if( ( ( gMstMt.cur_dir) && (gMtDisControlCheck.mstMtBackSpeed.lvVal>-4) )||
        ( (!gMstMt.cur_dir) && (gMtDisControlCheck.mstMtBackSpeed.lvVal< 4) ) )
    { //ʵ���ٶ�Ϊ0
        bMtCheckStop = true;
    }
    else
		{
        bMtCheckStop = false;
    }
    
    //�������õ��ٶ� �� ���٣�����ʵ����Ҫ�������ٶ� �� ���ٶȣ����ٶ�ֵ
    //Ȼ����µ��״̬
    DoLimitMstMtSpeed_Stop();
		
    if ( gMstMt.set_speed >= gMstMt.limit_speed )
    {   
        real_mst_mt_set_speed       = gMstMt.limit_speed;
        real_mst_mt_set_accl        = gSlvMtCfg.mstAddAccl/2 ;      //���ٶȼ���
        real_mst_mt_set_del_accl    = gSlvMtCfg.mstDelAccl*6 ;    //���ٶȼӿ�
        
        //�����ٶ�Ϊ0
        if( gMstMt.limit_speed == 0 )
				{
            bMtCheckStop = true;  //ǿ��ʶ��Ϊֹͣ״̬
        }
    }
    else
    {  
        real_mst_mt_set_speed       = gMstMt.set_speed;
        real_mst_mt_set_accl        = gSlvMtCfg.mstAddAccl ;    
        real_mst_mt_set_del_accl    = gSlvMtCfg.mstDelAccl ;
    }
    
    //�����ǰ��⵽�����ʧ�أ���Ҫǿ�ƹر������һ��ʱ��
    if( gMtDisControlCheck.mstNeedStopCnt > 0 )
		{
      gMtDisControlCheck.mstNeedStopCnt--;
      real_mst_mt_set_speed    = 0 ;
      real_mst_mt_set_del_accl = gSlvMtCfg.mstDelAccl*100 ;    //���ٶȼӿ�
			debug_sprintf(ID_DEBUG,"Master Motor out of control");
    }  

    //�趨�ٶȷ��� �뵱ǰʵ����� �ٶȺͷ���ͬ����Ҫ�����л�����
    //���жϿ��Ʒ��� �� ��ǰ����˶������Ƿ���ͬ
    if( gMstMt.set_dir != gMstMt.cur_dir )  //��Ҫ�л�����Ĵ������
    {   
        if( bMtCheckStop )
				{ //ʵ���ٶ�Ϊ0
            gMstMt.cur_speed = 11*100;
            gMstMt.cur_dir = gMstMt.set_dir;
        }
        else
				{ 	//�����ǰ��һ���趨�ٶȣ����ռ��ٶ�ֵ���м���
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
		{ 	//�ٶ��б仯
				if ( real_mst_mt_set_speed > gMstMt.cur_speed )   //��Ҫ����
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
        else  //real_mst_mt_set_speed < gMstMt.cur_speed  ��Ҫ���� �����ٵ����õ�ֵ
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
        if ( gMstMt.limit_speed_time )  //�����ϣ�����ֹͣ
        {
					//Send_Power_Board_Master_State(DISABLE);
					SET_MT_BREAK_CLOSE ;
        }
        else
				{ //�������ϣ��ȴ�ֹͣ
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
    SET_MASTER_MOTOR_PWM( real_mst_mt_set_speed );  //�ٶ�ֵ
		
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
*Return:		ʵ�����õ� ���ٶ�
*DESCRIPTION:  ���� �����ֵ���ļ��ٶ�  ���ٶ�ֵΪ���Ƶ���ٶȱ仯ʱ��
*           ��������θ���ʱ���仯���ٶ��� �ٶ�ֵΪ 0.00��100.00 
*           Ϊ�˿��� �ͼ��㾫�� ���ٶ�ֵ ��λΪ0.01
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
*DESCRIPTION:  �������ٶȴ�״̬��⺯��
*************************************************/
void zt_motor_master_wheel_slippage(void)
{
	signed short befor_master_position;
	signed short after_master_position;
	//char Master_Motor_State[8];  //������״̬��N:normal,����״̬��    	  	S:slip, ��״̬
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





