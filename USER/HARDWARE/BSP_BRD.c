
#include <includes.h>

#include <math.h>
#include "string.h"

//********************全局变量*********************//
u8 Rec_Data[UART_FIFO_NUM];
u8 USART_Flag;
u32 USARTFrameLength;
u16 GY26_cnt = 0;


//******************全局变量END********************//

/************************************************************************/
/*********************************电源控制板*****************************/
/**/
/**/
/**/
/************************************************************************/

OS_EVENT	*Uart2Sem;					//定义电源板信息  信号量

void USART_CMD_Judgement(void);
extern void USART_CMD_Judgement(void);

BRD_Info_Typedef BRD_Info; 
BAT_Protect_Info_Typedef				BAT_Protect_Info;
Power_Control_Status_Typedef 		Power_Control_Status;
Power_Charge_Status_Typedef     Power_Charge_Status;
Power_Cell_Balance_Status_Typedef  Power_Cell_Balance_Status;


Power_UARTbuf_Typedef Power_UART_Sendbuf;
Power_UARTbuf_Typedef Power_UART_Rx_Buf;


/*
  * @brief  BRD初始化函数，BRD在使用前必须先调用此函数
  * @param  无
  * @retval 无
*/
void  bsp_BRD_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 UART2 TX RX  I/O口 时钟 */
	RCC_AHB1PeriphClockCmd( BRD_UART_RX_GPIO_CLK|BRD_UART_TX_GPIO_CLK, ENABLE);

	/* 使能 UART2 时钟 */
	RCC_APB1PeriphClockCmd(BRD_UART_CLK, ENABLE);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(BRD_UART_RX_GPIO_PORT,BRD_UART_RX_SOURCE, BRD_UART_RX_AF);

	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(BRD_UART_TX_GPIO_PORT,BRD_UART_TX_SOURCE,BRD_UART_TX_AF);

	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = BRD_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRD_UART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = BRD_UART_RX_PIN;
	GPIO_Init(BRD_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置串口BRD_UART 模式 */
	USART_InitStructure.USART_BaudRate = BRD_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BRD_UART, &USART_InitStructure); 

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(BRD_UART, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */

	/*配置串口接收中断*/
	USART_ITConfig(BRD_UART, USART_IT_RXNE, ENABLE);

	//Uart2Sem = OSSemCreate(1);
	
	USART_Cmd(BRD_UART, ENABLE);
	
}

	 
void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    USART_SendData(USARTx, Data);
		//掉电、待机等特殊情况下，保证USART数据会成功发送出去
		//while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}



//包含包头到数据区所有数据的按字节的求和值，取低8位
u8 Power_Checksum(u8 Cmd,u8 Datalen,u8*data)
{
	u8 i,Packet_Length;
	u32 Checksum;
	Packet_Length = Datalen + 5;
	Checksum  = USART_HEAD_1 + USART_HEAD_2 ;
	Checksum  = Checksum + Packet_Length + Cmd;
	for(i = 0 ; i < Datalen ; i++)
	{
		Checksum += data[i];
	}
	return (Checksum & 0x00FF);
}



/*
  * @brief  BRD 串口中断处理函数
  * @param  无
  * @retval 无
*/
void BRD_POWER_ISR(void)
{
	//		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//				if(((RBDown2-RBUp2) & UARTNunD)!=1)
//				{    Uart2Recv[RBUp2++]=USART_ReceiveData(USART2);
//						 if(RBUp2>= UARTNunD)
//						 {
//								RBUp2=0;
//						 }
//				}
//			  else
//			  {
//					   Uart2Recv[RBUp2]=USART_ReceiveData(USART2);
//			  }	
//		} 
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		Rec_Data[GY26_cnt]=USART_ReceiveData(USART2);
		if(Rec_Data[0] != USART_HEAD_1) 
		 {
		 GY26_cnt = 0;
		 USARTFrameLength = 0;
		 }
		else 
		{
		  if(GY26_cnt == 1)
		   {
		     if(Rec_Data[1] != USART_HEAD_2) 
			  {
					if(Rec_Data[1] == USART_HEAD_1)
					{
            GY26_cnt = 1;
          }
					else
					{
			     GY26_cnt = 0;
			     USARTFrameLength = 0;
					}
			  }
			  else GY26_cnt++;
		   }
			else if(GY26_cnt == (5-3))   //get all lenth
			{
			  USARTFrameLength = Rec_Data[2];  //*(u32 *)(&Rec_Data[2]);  //2-5 length data
				//USARTFrameLength = (u32)Rec_Data[4] + (((u32)Rec_Data[5])<<8) + (((u32)Rec_Data[6])<<16) + (((u32)Rec_Data[7])<<24);
				if(USARTFrameLength > UART_FIFO_NUM) USARTFrameLength = UART_FIFO_NUM;
				GY26_cnt++;
			//	if(USARTFrameLength == 0x08) USARTFrameLength = 0x09;
			}
		  else if(GY26_cnt == USARTFrameLength - 1)
		  {
				//待加入CRC部分判断
		    USART_Flag =1;  
				//Switch_SPK(ON);
			  GY26_cnt = 0;
		  }
		  else 	GY26_cnt++;
		} 
	   if(GY26_cnt > UART_FIFO_NUM-1) GY26_cnt = 0;
	}	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TXE); 
	
	}
}


/*
* @brief  向电源板发送一条指令
  * @param  无
  * @retval cmd 命令字  datalen 数据长度 data数据
*/
//bsp_Power_UART_TX(GET_STATUS,0,0); 							//获取设备状态指令01
//bsp_Power_UART_TX(GET_PROTECT_CMD,0,0); 				//获取保护参数指令02
//bsp_Power_UART_TX(SET_PW_STATUS,0,0); 					//控制电源状态指令03
//bsp_Power_UART_TX(SET_CHARGE_STATUS,0,0); 			//控制充电开关指令04
//bsp_Power_UART_TX(SET_CELL_BALANCE,0,0); 				//控制平衡开关指令05
void bsp_Power_UART_TX(u8 Cmd,u8 Datalen,u8*data)
{
	u8 i,Checksum;
	UART_Tx_Byte(BRD_UART,USART_HEAD_1);
	UART_Tx_Byte(BRD_UART,USART_HEAD_2);
	UART_Tx_Byte(BRD_UART,Datalen + 5);
	UART_Tx_Byte(BRD_UART,Cmd);

	for(i = 0 ; i < Datalen ; i++)
	{
		UART_Tx_Byte(BRD_UART,data[i]);
	}
	
	Checksum  = Power_Checksum(Cmd, Datalen,data);
	
	UART_Tx_Byte(BRD_UART,Checksum);
}


/* 小端模式， 传输数据时， 低字节在前面*/
void Send_USART_CMD(u8 cmd, u8* databuf, u8 length)
{
  u8 checksumtmp;
  u8 i;
  checksumtmp =	USART_HEAD_1 + USART_HEAD_2 + cmd +	length;
  for(i=0;i<length-(8-3);i++)
   {
    checksumtmp += databuf[i];
   }
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2, USART_HEAD_1);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2, USART_HEAD_2);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2, length);
//  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//  USART_SendData(USART2,0);
//  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//  USART_SendData(USART2,0);
//  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//  USART_SendData(USART2,0);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2,cmd);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  for(i=0;i<length-(8-3);i++)
   { 
    USART_SendData(USART2,databuf[i]);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
   }
  USART_SendData(USART2,checksumtmp);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
} 


//串口接收解析函数
void USART_CMD_Judgement(void)
{
   u8 i,checksum;
   u8 datatmp[30] = {0};
   if(USART_Flag)
    {
	  checksum = 0;
	  for(i=0;i<USARTFrameLength-1;i++)
	  {
		   checksum += Rec_Data[i];
		}
	  if(Rec_Data[USARTFrameLength-1] == checksum)   //校验成功
	  {
		 switch(Rec_Data[6-3])   //类别帧
		 {
		  	case UART_CMD_ACK :	       datatmp[0] = 0;   //prepare to send err code or warning
			                             //Send_USART_CMD(UART_CMD_ACK,datatmp,8-3+0);
			                             break;

	      case UART_REPLY_GET_STATUS:      
																	 BRD_Info.Total_voltage = Rec_Data[5];    //total V
			                             BRD_Info.Total_voltage = ((BRD_Info.Total_voltage << 8) | Rec_Data[4]);
				                           BRD_Info.Electric_current = Rec_Data[7];    //total A
			                             BRD_Info.Electric_current = ((BRD_Info.Electric_current << 8) | Rec_Data[6]);
				                           BRD_Info.BTemp[0] =  Rec_Data[9];    //tem1
			                             BRD_Info.BTemp[0] =	 ((BRD_Info.BTemp[0] << 8) | Rec_Data[8]);
																	 BRD_Info.BTemp[1] =  Rec_Data[11];    //tem2
			                             BRD_Info.BTemp[1] =	 ((BRD_Info.BTemp[1] << 8) | Rec_Data[10]);
				                           BRD_Info.Monomer_voltage[0] = Rec_Data[13];    //cell1
			                             BRD_Info.Monomer_voltage[0] = ((BRD_Info.Monomer_voltage[0] << 8) | Rec_Data[12]);  
				                           BRD_Info.Monomer_voltage[1] = Rec_Data[15];    //cell2
			                             BRD_Info.Monomer_voltage[1] = ((BRD_Info.Monomer_voltage[1] << 8) | Rec_Data[14]);   
				                           BRD_Info.Monomer_voltage[2] = Rec_Data[17];    //cell3
			                             BRD_Info.Monomer_voltage[2] = ((BRD_Info.Monomer_voltage[2] << 8) | Rec_Data[16]);  
				                           BRD_Info.Monomer_voltage[3] = Rec_Data[19];    //cell4
			                             BRD_Info.Monomer_voltage[3] = ((BRD_Info.Monomer_voltage[3] << 8) | Rec_Data[18]);
																	 BRD_Info.Monomer_voltage[4] = Rec_Data[21];    //cell5
			                             BRD_Info.Monomer_voltage[4] = ((BRD_Info.Monomer_voltage[4] << 8) | Rec_Data[20]);
				                           BRD_Info.Monomer_voltage[5] = Rec_Data[23];    //cell6
			                             BRD_Info.Monomer_voltage[5] = ((BRD_Info.Monomer_voltage[5] << 8) | Rec_Data[22]);
																	 BRD_Info.Sw_Status = Rec_Data[24];    //bk_sta  motor sta cam24_sta cam12_sta
																	 BRD_Info.BMS_Protect_Status = Rec_Data[25];     //历史bms保护故障标志
																	 BRD_Info.Change_MOS_Status = Rec_Data[26]; 		 //充放电MOS状态
																	 BRD_Info.Battery_Balance_Status = Rec_Data[27];  //充放电平衡状态
																	 BRD_Info.Accel_Value_X = Rec_Data[29];
																	 BRD_Info.Accel_Value_X = ((BRD_Info.Accel_Value_X << 8) | Rec_Data[28]);
																	 BRD_Info.Accel_Value_Y = Rec_Data[31];
																	 BRD_Info.Accel_Value_Y = ((BRD_Info.Accel_Value_Y << 8) | Rec_Data[30]);
																	 BRD_Info.Accel_Value_Z = Rec_Data[33];
																	 BRD_Info.Accel_Value_Z = ((BRD_Info.Accel_Value_Z << 8) | Rec_Data[32]);
					                         //Send_USART_CMD(UART_REPLY_GET_STATUS,datatmp,8-3+30);  //back ack cmd
																	 //nprintf(ID_DEBUG,datatmp[0],0,DEC);
					                         break;
				case UART_REPLY_GET_PROTECT: 
					                         BAT_Protect_Info.Short_Protect_Current_threshold = Rec_Data[4];
																	 BAT_Protect_Info.Overload_Protect_Current_threshold = Rec_Data[5];
																	 BAT_Protect_Info.Monomer_overvoltage_Protect_threshold = Rec_Data[7];
																	 BAT_Protect_Info.Monomer_overvoltage_Protect_threshold = ((BAT_Protect_Info.Monomer_overvoltage_Protect_threshold << 8) | Rec_Data[6]);
				                           BAT_Protect_Info.Monomer_Undervoltage_Protect_threshold = Rec_Data[9];
																	 BAT_Protect_Info.Monomer_Undervoltage_Protect_threshold = ((BAT_Protect_Info.Monomer_Undervoltage_Protect_threshold << 8) | Rec_Data[8]);
				                           BAT_Protect_Info.Discharge_capacity_Protect_threshold = Rec_Data[11];
																	 BAT_Protect_Info.Discharge_capacity_Protect_threshold = ((BAT_Protect_Info.Discharge_capacity_Protect_threshold << 8) | Rec_Data[10]);
					                         //Send_USART_CMD(UART_REPLY_GET_PROTECT,datatmp,8-3+8);  //
					                         break;
		    case UART_REPLY_SET_PW   : 
																		Power_Control_Status.Power_SW_Status = Rec_Data[4];
				                            
					                          if(Power_Control_Status.Power_SW_Status & (1<<3))
				                            {
																		  Power_Control_Status.Mst_Brake_Status_Flag = 1;
																		}
																		else
																		{
																			Power_Control_Status.Mst_Brake_Status_Flag = 0;
																		}
																		if(Power_Control_Status.Power_SW_Status & (1<<2))
				                            {
																		  Power_Control_Status.Motor_Power_Status_Flag = 1;
																		}
																		else
																		{
																		  Power_Control_Status.Motor_Power_Status_Flag = 0;
																		}
																		if(Power_Control_Status.Power_SW_Status & (1<<1))
				                            {
																		  Power_Control_Status.Cam_Power_24V_Status_Flag = 1;
																		}
																		else
																		{
																		  Power_Control_Status.Cam_Power_24V_Status_Flag = 0;
																		}
																		if(Power_Control_Status.Power_SW_Status & (1<<0))
				                            {
																		  Power_Control_Status.Cam_Power_12V_Status_Flag = 1;
																		}
																		else
																		{
																		  Power_Control_Status.Cam_Power_12V_Status_Flag = 0;
																		}
																	 //Send_USART_CMD(UART_REPLY_SET_PW,datatmp,8-3+1);  //
					                         break;
				case UART_SET_CHARGE_STATUS: 
					                          if(Rec_Data[4] == 0x01)
																		{
																			Power_Charge_Status.Bat_Charge_Enable_Flag = 1;
																		}
																		else
																		{
																			Power_Charge_Status.Bat_Charge_Enable_Flag = 0;
																		}
					                          
					                          //Send_USART_CMD(UART_CMD_ACK,datatmp,8-3);  //
				                            break;
				case UART_SET_CELL_BALANCE: 													
																		if(Rec_Data[4] == 0x01)
																		{
																		   Power_Cell_Balance_Status.Cell_Balance_Enable_Flag = 1;
																		}
																		else
																		{
																		   Power_Cell_Balance_Status.Cell_Balance_Enable_Flag  = 0;
																		}
																		//Send_USART_CMD(UART_CMD_ACK,datatmp,8-3);  //
                                    break;					
			 default: 
				break;
		  }
		}	   	 
	 USART_Flag = 0;   //括号前最后清零
	}
} 



//获取电源板信息参数
void STM_GET_POWER_Info(void)
{

	//bsp_BRD_Send_Cmd(STM_TX_GET_BAT_STATUS,sizeof(BRD_Info),0,(u8 *)(&BRD_Info));	
	
}

BRD_Info_Typedef gBattery_Value;

//获取电池电压值
short POWER_get_BAT_VOL(void)
{
	gBattery_Value.Total_voltage = BRD_Info.Total_voltage;
	return gBattery_Value.Total_voltage;
}

//获取电池电流值
short POWER_get_BAT_CUR(void)
{
  gBattery_Value.Electric_current = BRD_Info.Electric_current;
	return gBattery_Value.Electric_current;
}


