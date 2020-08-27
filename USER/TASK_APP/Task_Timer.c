
#define IN_TASK_TIMER

#include "includes.h"

//打印输出函数
#define  DEBUG   1  
#if DEBUG
#define DBG_PRINTF(fmt, args...)\
do\
{\
	printf("<<File:%s  Line:%d  Function:%s Date:%s Time:%s >> ", __FILE__, __LINE__, __FUNCTION__,__DATE__,__TIME__);\
	printf(fmt, ##args);\
}while(0)
#else
  #define DBG_PRINTF(fmt, args...)   
#endif


/*************************************************/
//---------本任务涉及到的消息-----------
OS_EVENT *timerQueue;

ZT_INFO_TYPE g_zt_msg;

OS_TMR   * tmr1_temper_humidi;			//软件定时器1 温湿度采集	
OS_TMR   * tmr2_sys_led;			      //软件定时器2 系统指示灯

//软件定时器1的回调函数		温湿度采集	   
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


//软件定时器2的回调函数		系统指示灯	   
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


//向Linux发送从轮子码盘值，
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
	buf[13]=(gRbtState.bCX_SwCheck1 || gRbtState.bCX_SwCheck2)? 0x01 : 0x00 ; //利用红外状态判断充电状态
	buf[14]=0x96;

	ack_with_debug(ID_LINUX,buf,15);	//将码盘值发送到ID_LINUX串口3
	
}




void System_State_LED(void)

{
	
	//板载指示灯
   LED1(LED_ON);
	 OSTimeDly(OS_TICKS_PER_SEC/4);
	 
	 LED1(LED_OFF);
	 OSTimeDly(OS_TICKS_PER_SEC/4);
	
	

}


/*==================================

	函数：	Clear_Buffer
	说明：	清零一段缓冲区
	入口：	*pointer--源指针
			Length----清零长度
	出口：	无
	
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

	函数：	CopyBuffer
	说明：	从源内存区向目标内存区顺序拷贝一定长度的内存块
	入口：	*SouPointer--源指针
			*DesPointer--目标指针
			Length-------拷贝长度
	出口：	返回目的区的指针
	
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

	 if(com==ID_RF433)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
 	 }
	 else if(com==ID_POWER_BOARD)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_POWER_BOARD;
 	 }
	 else if(com==ID_LINUX)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
 	 }
     else if(com==ID_DEBUG)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
 	 }
	 
	 dp1->point =  (uint8 *)dp;
	 dp1->count =  StringLen(str);
	 if(OS_ERR_NONE != OSQPost(GetWeatherQueue,(void*)dp1))
	 {
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
		case 0:						 					  //十进制显示
		for(i=1000000000;i>1;i/=10)	  //转化成ascll码
		{
		  *cp=num/i+'0';
		  num%=i;
		  cp++; 
		} 
		*cp=num+'0';
		cp++;
		*cp='\0';					  //加字符串结束标志
		if(mul)
		{
		   for(i=0;i<mul+1;i++)
		   {
			   *(cp+1)=*cp;
			   cp--;
		   }
		   *(cp+1)='.'; 			 //根据倍数添加小数点
		}
		cp=dp;
		while(*cp!='\0') 			  //去掉打头的 0
		{
		   if(*cp!='0') break;
		   cp++;
		}
		if( *cp=='.'|| *cp=='\0')  cp--;
	  	CopyBuffer(cp,dp,StringLen(cp));
		
		if(com==ID_RF433)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
 	 }
	 else if(com==ID_POWER_BOARD)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_POWER_BOARD;
 	 }
	 else if(com==ID_LINUX)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
 	 }
     else if(com==ID_DEBUG)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
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
		for( ;i>1;i>>=4) 			 //转化成ascll码
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
		*cp='\0';					  //加字符串结束标志
		cp=dp;
		CopyBuffer(cp,dp,StringLen(cp));
		if(com==ID_RF433)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_RF433;
 	 }
	 else if(com==ID_POWER_BOARD)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_POWER_BOARD;
 	 }
	 else if(com==ID_LINUX)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_LINUX;
 	 }
     else if(com==ID_DEBUG)
 	 {
		 dp1->type = WEATHER_DEBUG_OUT_MSG_DEBUG;
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

//SWD模式下载初始化函数，系统启动后，
//需要加入此函数，不然无法使用SWD模式下载。
void SWD_Download_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*使能GPIOA时钟*/
	//开启下载功能，复用引脚
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_SWJ);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_SWJ);
	//GPIO 初始化 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
   
  //配置PA13引脚为复用功能
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //配置PA14引脚为复用功能
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}


//时间 定时器任务入口函数
void Task_Timer(void *pdata)
{
	uint8 err;
	DATA_CONTROL_BLOCK *msg;
	
	static int gPress_Newton;
	static s32 nCxCheckMove;
	
	//创建温湿度定时器1，
	tmr1_temper_humidi=OSTmrCreate(0,1000,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_temper_humidi_callback,0,"tmr1_temper_humidi",&err);		
	OSTmrStart(tmr1_temper_humidi,&err);			//启动软件定时器1，2ms*1000执行一次
	
	//创建指示灯定时器2 
	tmr2_sys_led=OSTmrCreate(0,500,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_sys_led_callback,0,"tmr2_sys_led",&err);		
	OSTmrStart(tmr2_sys_led,&err);			//启动软件定时器2，2ms*500执行一次
	
	//SWD模式下载初始化函数，系统启动后，需要加如此函数，不然无法使用SWD模式下载。
	SWD_Download_init();		
	
	while( 1 )
	{
		msg = (DATA_CONTROL_BLOCK *)OSQPend(timerQueue,0,&err);
		
		if ( err == OS_ERR_NONE )			//队列接收到信号，返回值 msg不为零，err=0
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
					gPress_Newton = Pressure_Sensor_Get_Voltage();
					nprintf(ID_DEBUG,gPress_Newton,0,DEC);
					//stprintf(ID_LINUX,"LED \r\n");
					//DBG_PRINTF("Hello World");
				}
				break;
				case BATTERY_MSG: 		// 256ms
				{
					//LED2(LED_ON);
					//Battery();	
					
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
				 case SENSOR_MSG: 		//4ms
				{
					//LED2(LED_ON);
					Sensor_Collect();
					break;
				}
				 
				default:
				break;
			 }		
			OSMemPut(p_msgBuffer,(void*)msg);
		}	
	}

}





