
#define IN_TASK_TIMER

#include "includes.h"


/*************************************************/
//---------�������漰������Ϣ-----------
OS_EVENT *timerQueue;
//RbtState gRbtState;
//BatAutoCtrl gBatAutoCtrl;
//SlvMtCfgType     gSlvMtCfg;  		 //���ò���
ZT_INFO_TYPE g_zt_msg;

//robot��Ϣ�ṹ��
//int  gCmdParaVal_Ints[8];
//CmdParaInfo gCmdParaInfos;

OS_TMR   * tmr1_temper_humidi;			//�����ʱ��1 ��ʪ�Ȳɼ�	
OS_TMR   * tmr2_sys_led;			      //�����ʱ��2 ϵͳָʾ��

//�����ʱ��1�Ļص�����		��ʪ�Ȳɼ�	   
void tmr1_temper_humidi_callback(OS_TMR *ptmr,void *p_arg) 
{  
		INT8U  err;
		DATA_CONTROL_BLOCK  *dp1;
		dp1= OSMemGet(p_msgBuffer,&err);
		//stprintf(ID_DEBUG,"time1 \r\n");
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp1)->type = AM2320_MSG; 
			if(OS_NO_ERR != OSQPost(timerQueue,(void*)dp1)) 	 
			{
				OSMemPut(p_msgBuffer,dp1);
			}
		} 	   
}


//�����ʱ��2�Ļص�����		ϵͳָʾ��	   
void tmr2_sys_led_callback(OS_TMR *ptmr,void *p_arg) 
{  
		INT8U  err;
		DATA_CONTROL_BLOCK  *dp2;
		dp2= OSMemGet(p_msgBuffer,&err);
		//stprintf(ID_DEBUG,"time1 \r\n");
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp2)->type = SYS_LED_MSG; 
			if(OS_NO_ERR != OSQPost(timerQueue,(void*)dp2)) 	 
			{
				OSMemPut(p_msgBuffer,dp2);
			}
		}  	   
}


//��Linux���ʹ���������ֵ��
void SendWHLToLinux(void)
{
	u8 buf[15];
    signed long temp0;
	static unsigned long fn;
	
    fn++;

	//temp0=(signed long)(GET_SLAVE_WHEEL_CODE());
	temp0=gBatAutoCtrl.nCxCheckMove;
	//temp0=9876;///////////////////////////////////////test
	buf[0]=0xa5;
	buf[1]=0x5a;
	buf[2]=0x04;
	buf[3]=0x00;
	buf[4]=0xb1;
	buf[5]=(temp0 & 0x000000ff);
	buf[6]=(temp0 & 0x0000ff00)>>8;
	buf[7]=(temp0 & 0x00ff0000)>>16;
    buf[8]=(temp0 & 0xff000000)>>24;
	buf[9]=(fn  & 0x000000ff);
	buf[10]=(fn & 0x0000ff00)>>8;
	buf[11]=(fn & 0x00ff0000)>>16;
	buf[12]=(fn & 0xff000000)>>24;
	buf[13]=(gRbtState.bCX_SwCheck1 || gRbtState.bCX_SwCheck2)? 0x01 : 0x00 ; //���ú���״̬�жϳ��״̬
	buf[14]=0x96;

	ack_with_debug(ID_LINUX,buf,15);	//������ֵ���͵�ID_LINUX���ڶ�
	
}



//��ص�ѹ����ص���  ADC�ɼ����˲��������
void DoBatUsingGetFilter(void)
{
    static u8  iVolId = 0;
    static u16 nVolValues[5]={0,0,0,0,0} ; 
    static u16 nVolAll=0; //
    //static u8  iCurId = 0;
    static u16 nCurValues[5]={0,0,0,0,0} ; 
    static u16 nCurAll=0; //
    
    if(iVolId>=5){ iVolId = 0; }
    nVolAll -= nVolValues[iVolId];
    nCurAll -= nCurValues[iVolId];
    nVolValues[iVolId] = get_adc_val(ADC_TYPE_ID_BAT_VOT);
    nCurValues[iVolId] = get_adc_val(ADC_TYPE_ID_BAT_CUR);
    nVolAll += nVolValues[iVolId];
    nCurAll += nCurValues[iVolId];
	//ȡ��ֵ���˲�
    gRbtState.nBatVol = nVolAll / 5 ;
    gRbtState.nBatCur = nCurAll / 5 ;
    iVolId++;
}

void Battery(void)
{
    //��������ڳ���У�gBatAutoCtrl.bCharging
    //uint8 ret;
    DoBatUsingGetFilter(); 
	//int ValCnt = cmd_para_get_int_by_val_str( gCmdParaInfos.items[0].value , gCmdParaVal_Ints , 5 );
	
    if( gBatAutoCtrl.bCharging )
	{
        gBatAutoCtrl.iChargingCircle++;
        if( gBatAutoCtrl.iChargingCircle <= (4*60) )        //���ʱ�䣺256ms * 4 *60 һ����
		{
			//stprintf(ID_DEBUG,"2\r\n");
			if(!gRbtState.bChargeShort)
			{ 
				BAT_CHARGE_OPEN(); 
				//stprintf(ID_DEBUG,"1\r\n");
				//nprintf(ID_DEBUG,gBatAutoCtrl.iChargingCircle,0,0);
			}
			//����������ϣ��յ�����˶�ָ����������ٶȲ�Ϊ0����������˶�
			
        }
        else if( gBatAutoCtrl.iChargingCircle <= (4*60+10) )	//���һ���ӣ��Ͽ�2S��������ѹ
		{
			//stprintf(ID_DEBUG,"2\r\n");
			BAT_CHARGE_CLOSE();
        }
        else //if(gBatAutoCtrl.iChargingCircle >= 0)
		{
            gBatAutoCtrl.curBatVol = BAT_VOL_FLOAT*10;
            if( gBatAutoCtrl.curBatVol > gSlvMtCfg.bat_charge_full )	//���ڳ�����ѹ���Ͽ��̵�����ֹͣ���
			{
                gBatAutoCtrl.bCharging = false; 
				BAT_CHARGE_CLOSE();
				//stprintf(ID_DEBUG,"4\r\n");
            }
            gBatAutoCtrl.iChargingCircle = 0;			
        }
       
        if( !gRbtState.bChargeVolIn )					//�޵�ѹ���룬ֹͣ���
		{
            gBatAutoCtrl.bCharging = false; 
			BAT_CHARGE_CLOSE();  						//�ⲿ����ر�
			//stprintf(ID_DEBUG,"5\r\n");
        }
       
    }
	else
	{
		
		gBatAutoCtrl.curBatVol = BAT_VOL_FLOAT*10;

	}

    g_zt_msg.nTimeForNoLinuxHeartIn++;
	
    //zt_build_send_state_string(BUILD_STATE_FLAG_ALL);//zs 1116 del
    //uart2_send(g_zt_msg.sendbuf , g_zt_msg.icmd_len );//zs 1116 del
	
    //uart1_send(g_zt_msg.sendbuf , g_zt_msg.icmd_len );
    //stprintf(ID_DEBUG,"send to linux=ok\r\n");

}



void System_State_LED(void)

{
	
	//����ָʾ��
   LED1(LED_ON);
	 delay_ms(200);
	 LED1(LED_OFF);
	 delay_ms(200);

}

enum
{
    LINUX_CMD50_HEAD            = 0X00,						//֡ͷλ
    LINUX_CMD50_PKG_LEN         ,
};



/*==================================

	������	Clear_Buffer
	˵����	����һ�λ�����
	��ڣ�	*pointer--Դָ��
			Length----���㳤��
	���ڣ�	��
	
===================================*/
void Clear_Buffer(void *pointer,uint16 length)
{
	uint16 i;
	uint8 *dp; 

	dp = (uint8 *)pointer;
	for (i=0;i<length;i++)
	{
		*(dp++) = 0x00;
	}
}


/*==================================

	������	CopyBuffer
	˵����	��Դ�ڴ�����Ŀ���ڴ���˳�򿽱�һ�����ȵ��ڴ��
	��ڣ�	*SouPointer--Դָ��
			*DesPointer--Ŀ��ָ��
			Length-------��������
	���ڣ�	����Ŀ������ָ��
	
===================================*/
void CopyBuffer(void *SouPointer,void *DesPointer,uint16 Length)
{
		
	uint16 i;
	uint8 *D_Pointer;
	uint8 *S_Pointer; 

	if(DesPointer < SouPointer) 
	{
		D_Pointer = (uint8 *)DesPointer;
		S_Pointer = (uint8 *)SouPointer;
		for (i = 0; i < Length; i++ )
		{
			*D_Pointer++ = *S_Pointer++ ;
		}
	}
	else 
	{
		D_Pointer = ((uint8 *)DesPointer)+(Length - 1);
		S_Pointer = ((uint8 *)SouPointer)+(Length - 1);
		for(i = 0; i < Length; i++ )
		{
			*D_Pointer-- = *S_Pointer--;
		}
	}

	//return DesPointer;
}

uint8 StringLen(char * Pointer)
{
	uint8 i=0;
	
	while((*Pointer++) != '\0')
	{
		i++;
	}
	return i;
}


void debug_sprintf(uint8 com,char *str)
{
	 uint8 err;
	 char *dp;
	 DATA_CONTROL_BLOCK *dp1;

	 
	 dp = (char *)OSMemGet(mem160ptr,&err);
	 if(err != OS_ERR_NONE)
	 {	
		 OSMemPut(mem160ptr,dp);
		 return;
	 }
	 
	 dp1 = OSMemGet(p_msgBuffer,&err);
	 if(err != OS_ERR_NONE)
	 {
	 	OSMemPut(p_msgBuffer,dp1);
		 return;
	 }
	 
	 CopyBuffer(str, dp, StringLen(str));

	 if(com==ID_DEBUG)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
 	 }
	 else if(com==ID_LINUX)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
 	 }
     else if(com==ID_RF433)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
 	 }
	 dp1->point =  (uint8 *)dp;
	 dp1->count =  StringLen(str);
	 if(OS_ERR_NONE != OSQPost(GetWeatherQueue,(void*)dp1))
	 {
     //GpioSetL(GPIO_LED_OUT_SHOW1);
		 OSMemPut(mem160ptr,dp);
		 OSMemPut(p_msgBuffer,dp1);
	 }
}

void debug_nprintf(uint8 com,uint32 num,uint8 mul,uint8 typ)
{
	 uint8 err;
	 uint32 i;
	 char *cp,*dp;
	 DATA_CONTROL_BLOCK *dp1;
	 
	 dp = (char *)OSMemGet(mem160ptr,&err);
	 if(err != OS_ERR_NONE)
	 {		
		 OSMemPut(mem160ptr,dp);
		 return;
	 }
	  
	 dp1 = OSMemGet(p_msgBuffer,&err);
	 if(err != OS_ERR_NONE)
	 {
	 	 OSMemPut(p_msgBuffer,dp1);
		 return;
	 }

	cp = dp;
	switch(typ)
	{		 
		case 0:						 					  //ʮ������ʾ
		for(i=1000000000;i>1;i/=10)	  //ת����ascll��
		{
		  *cp=num/i+'0';
		  num%=i;
		  cp++; 
		} 
		*cp=num+'0';
		cp++;
		*cp='\0';					  //���ַ���������־
		if(mul)
		{
		   for(i=0;i<mul+1;i++)
		   {
			   *(cp+1)=*cp;
			   cp--;
		   }
		   *(cp+1)='.'; 			 //���ݱ������С����
		}
		cp=dp;
		while(*cp!='\0') 			  //ȥ����ͷ�� 0
		{
		   if(*cp!='0') break;
		   cp++;
		}
		if( *cp=='.'|| *cp=='\0')  cp--;
	  	CopyBuffer(cp,dp,StringLen(cp));
		
		if(com==ID_DEBUG)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
	 	}
		else if(com==ID_LINUX)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
	 	}
	    else if(com==ID_RF433)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
	 	}
		dp1->point =  (uint8 *)dp;
		dp1->count =  StringLen(cp);
		
		if(OS_ERR_NONE != OSQPost(GetWeatherQueue,(void*)dp1))
		{
			 OSMemPut(mem160ptr,dp);
			 OSMemPut(p_msgBuffer,dp1);
		}
		break;
		
		case 1:
		i=0x10000000;
		if(num<65536) i=0x1000;
		if(num<256)	 i=0x10;
		for( ;i>1;i>>=4) 			 //ת����ascll��
		{
		  *cp=num/i;
		  if(*cp>9) *cp+='a'-10;
		  else		*cp+='0';
		  num%=i;
		  cp++; 
		}
		*cp=num;
		if(*cp>9) *cp+='a'-10;
		else 	 *cp+='0';
		cp++;
		*cp='\0';					  //���ַ���������־
		cp=dp;
		CopyBuffer(cp,dp,StringLen(cp));
		if(com==ID_DEBUG)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
	 	}
		else if(com==ID_LINUX)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
	 	}
	    else if(com==ID_RF433)
	 	{
			dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
	 	}
		dp1->point =  (uint8 *)dp;
		dp1->count =  StringLen(cp);
		if(OS_ERR_NONE != OSQPost(GetWeatherQueue,(void*)dp1))
		{
			OSMemPut(mem160ptr,dp);
			OSMemPut(p_msgBuffer,dp1);		
		}
		break;
		default:
		break;
	 
	 }
}


//ʱ�� ��ʱ��������ں���
void Task_Timer(void *pdata)
{
	uint8 err;
	DATA_CONTROL_BLOCK *msg;
	static s32 nCxCheckMove;
	
	//������ʪ�ȶ�ʱ��1��
	tmr1_temper_humidi=OSTmrCreate(0,1000,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_temper_humidi_callback,0,"tmr1_temper_humidi",&err);		
	OSTmrStart(tmr1_temper_humidi,&err);			//���������ʱ��1��2ms*1000ִ��һ��
	
	//����ָʾ�ƶ�ʱ��2 
	tmr2_sys_led=OSTmrCreate(0,500,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_sys_led_callback,0,"tmr2_sys_led",&err);		
	OSTmrStart(tmr2_sys_led,&err);			//���������ʱ��2��2ms*500ִ��һ��
	
	while( 1 )
	{
		msg = (DATA_CONTROL_BLOCK *)OSQPend(timerQueue,0,&err);
		
		if ( err == OS_ERR_NONE )			//���н��յ��źţ�����ֵ msg��Ϊ�㣬err=0
		{
			switch ( msg->type )
			{
				case AM2320_MSG: 		// 2s
				{
					//LED2(LED_ON);
					AM2320_Get_Data();
					//stprintf(ID_DEBUG,"AM2320 \r\n");				
				}
				break;
				case SYS_LED_MSG: 		// 1s
				{
					//LED2(LED_ON);
					System_State_LED();	
					
				}
				break;
				case BATTERY_MSG: 		// 256ms
				{
					//LED2(LED_ON);
					Battery();	
					
				}
				break;
        case WHL_MSG:     		//32ms
				{
					//LED2(LED_ON);
					//if(((nCxCheckMove-gBatAutoCtrl.nCxCheckMove)>8)||((gBatAutoCtrl.nCxCheckMove-nCxCheckMove)>8))
					if(nCxCheckMove!=gBatAutoCtrl.nCxCheckMove)
           {
              nCxCheckMove=gBatAutoCtrl.nCxCheckMove;
							SendWHLToLinux();
						}					
				 }
				break;
				default:
				break;
			 }		
			OSMemPut(p_msgBuffer,(void*)msg);
		}	
	}

}





