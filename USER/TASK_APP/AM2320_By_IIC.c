
#include <includes.h>

short int gnAM2320_temper = 0;
short int gnAM2320_humidi = 0;




I2C_INIT_INFO I2C_Info;	///<I2C控制器相关信息

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  硬件I2C初始化 。
  * @param  I2C_Info I2C初始化信息。
  * @retval 初始化数据状态。
  */
uint8 VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info)
{
	I2C_InitTypeDef  I2C_InitStructure;
   	GPIO_InitTypeDef  GPIO_InitStructure;
   	if(0 == I2C_Info->channel)
	{
			//  I2C3_SCL:PA8 I2C3_SDA：PC9;
	   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC , ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE);
	
	   	/* Configure I2C3 pins: PA8->SCL and PC9->SDA */
	   	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	   	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	   	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
	   	I2C_DeInit(I2C3);
	   	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	   	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	   	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	   	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		if(I2C_Info->slaveAddr>>8)
		{
			I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
		}
		else
		{
	   		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		}
	   	I2C_InitStructure.I2C_ClockSpeed = I2C_Info->speed;
	    
	   	I2C_Cmd(I2C3, ENABLE);
	   	I2C_Init(I2C3, &I2C_InitStructure);
	
	   	I2C_AcknowledgeConfig(I2C3, ENABLE);
   	}
	
	else
	{
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
//			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB  , ENABLE);
//	
//	   	/* Configure I2C1 pins: PB10->SCL and PB11->SDA */
//	   	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
//	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
//	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	   	GPIO_Init(GPIOB, &GPIO_InitStructure);
//			
//	   	I2C_DeInit(I2C2);
//	   	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	   	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//	   	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
//	   	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//		if(I2C_Info->slaveAddr>>8)
//		{
//			I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
//		}
//		else
//		{
//	   		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//		}
//	   	I2C_InitStructure.I2C_ClockSpeed = I2C_Info->speed;
//	    
//	   	I2C_Cmd(I2C2, ENABLE);
//	   	I2C_Init(I2C2, &I2C_InitStructure);
//	
//	   	I2C_AcknowledgeConfig(I2C2, ENABLE);
	}
	return 0;
}
/**
  * @brief  通过硬件I2C写数据 。
  * @param  I2Cx 硬件I2C通道。
  * @param  subaddr 写数据的子地址，若不存在子地址则该数据无意义。
  * @param  s 需要写的数据缓冲区地址。
  * @param  num 需要写的数据字节数。
  * @retval 写数据状态。
  */
uint8 VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num)
{
  	uint32_t subaddrNum=I2C_Info.subAddrWidth;
	uint32_t retry=0;
	uint8_t *p_data=s;
	I2C_GenerateSTART(I2Cx, ENABLE);   //产生一个起始信号
	retry=0;
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		retry++;
		if(retry>=1000)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_MODE_SELECT_FAILED;	
		}
	}
	//发送设备地址
	if(I2C_Info.slaveAddr>>8)
	{
  		//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
	}
	else
	{
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Transmitter);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);   //产生一个停止信号
				return MASTER_TRANSMITTER_MODE_SELECTED_FAILED;	
			}
		}
	}
  	
	//发送子地址
	while(subaddrNum>0)
	{
	  	I2C_SendData(I2Cx, subaddr>>((subaddrNum-1)*8));
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);		//产生一个停止信号
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
		subaddrNum--;
	}
	//发送数据
	while(num--)
	{
	  	I2C_SendData(I2Cx, *p_data++); 
		retry=0;
  		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);			//产生一个停止信号
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
	}
	//停止总线
	I2C_GenerateSTOP(I2Cx, ENABLE);
	return 0;
}
/**
  * @brief  通过硬件I2C读数据 。
  * @param  I2Cx 硬件I2C通道。
  * @param  subaddr 读数据的子地址，若不存在子地址则该数据无意义。
  * @param  s 需要读的数据存储缓冲区地址。
  * @param  num 需要读的数据字节数。
  * @retval 读数据状态。
  */
uint8 VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32_t subaddr,uint8_t *s,uint32_t num)
{
  	int32_t	subaddrNum=I2C_Info.subAddrWidth;
	uint8_t *p_data=s;	
	uint32_t retry=0;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
	{
		retry++;
		if(retry>=1000)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_GET_I2C_FLAG_BUSY_FAILED;	
		}
	}
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
    I2C_GenerateSTART(I2Cx, ENABLE);
	retry=0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		retry++;
		if(retry>=1000)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_MODE_SELECT_FAILED;	
		}
	}
	if(subaddrNum>0)
	{
		//发送设备地址
		if(I2C_Info.slaveAddr>>8)
		{
	  		//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
		}
		else
		{
			I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Transmitter);
			retry=0;
			while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			{
				retry++;
				if(retry>=1000)
				{
					I2C_GenerateSTOP(I2Cx, ENABLE);
					return MASTER_TRANSMITTER_MODE_SELECTED_FAILED;	
				}
			}
		} 
		//发送子地址
		while(subaddrNum>0)
		{
		  	I2C_SendData(I2Cx, subaddr>>((subaddrNum-1)*8));
			retry=0;
			while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{
				retry++;
				if(retry>=1000)
				{
					I2C_GenerateSTOP(I2Cx, ENABLE);
					return MASTER_BYTE_TRANSMITTED_FAILED;	
				}
			}
			subaddrNum--;
		}
		//重新启动I2C总线
		I2C_GenerateSTART(I2Cx, ENABLE);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_MODE_SELECT_FAILED;	
			}
		}
		//发送读设备地址
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Receiver);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_RECEIVER_MODE_SELECTED_FAILED;	
			}
		}
	}
	else
	{
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Receiver);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_RECEIVER_MODE_SELECTED_FAILED;	
			}
		}
	}
	//读取数据
    while (num)
    {
		if(num==1)
		{
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);
    		I2C_GenerateSTOP(I2Cx, ENABLE);
		}
	    retry = 0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_BYTE_RECEIVED_FAILED;	
			}
		}
	    *p_data = I2C_ReceiveData(I2Cx);
	    p_data++;
	    /* Decrement the read bytes counter */
	    num--;
    }

	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	return 0;
}





/**
  * @brief  延时函数
  * @param  Time 延时时间参数
  * @retval None
  */
void Delay(uint32_t Time)
{
	while(Time--);
}

/************************************************* 
*Function:		AM2320_and_iic_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  AM2320 芯片状态初始化 以及系统初始化
*************************************************/

extern I2C_INIT_INFO I2C_Info;	///<I2C控制器相关信息


void AM2320_and_iic_init(void)
{

	#define AM2320_ADDRESS 0xB8
	
	//uint8 AM2320_TX_Buffer[] = {0x03,0x00,0x04};//读温湿度命令(无CRC校验)
	//uint8 AM2320_RX_Buffer[12];
	
	//根据AM2320传感器设置I2C初始化数据
	I2C_Info.controlMode = 1;		//硬件控制
	I2C_Info.speed=100000;			//100K
	I2C_Info.subAddrMode=2;			//无子地址模式
	I2C_Info.subAddrWidth=0;		//子地址为0字节宽度
	I2C_Info.slaveAddr=AM2320_ADDRESS;		//AM2320传感器I2C地址
	I2C_Info.channel=0;				//使用I2C3
	VT_I2C_HardInit(&I2C_Info);
		
}


void AM2320_Get_Data(void)
{

	uint8		ret;
	uint8   AM2320Wake[2] ;
	uint8   Send_to_AM2320Data[8];
	uint8   Read_from_AM2320Data[10];
	
	AM2320Wake[0] = 0x00;
	VT_I2C_HardWriteNByte(I2C3,0,AM2320Wake,1);//随便发送一个数据出去，将传感器从睡眠中唤醒
	//OSTimeDly(1);
		
	Send_to_AM2320Data[0]=0x03;
	Send_to_AM2320Data[1]=0x00;
	Send_to_AM2320Data[2]=0x04;
	ret = VT_I2C_HardWriteNByte(I2C3,0,Send_to_AM2320Data,3);//发送读取温度和湿度的指令
	if(ret)
	{
		stprintf(ID_DEBUG,"I2C写数据错误:  ");
		nprintf(ID_DEBUG,ret,0,DEC);
		stprintf(ID_DEBUG,"\r\n");
		
	}
	OSTimeDly(2);
	//Delay(100000);//必须加这个延时，否则传感器没准备好数据会出现读数据错误
	//GpioSetL(GPIO_LED_SHOW1);
	ret = VT_I2C_HardReadNByte(I2C3,0,Read_from_AM2320Data,8);//读取温度和湿度
	if(ret)
	{
		stprintf(ID_DEBUG,"I2C写数据错误:  ");
		nprintf(ID_DEBUG,ret,0,DEC);
		stprintf(ID_DEBUG,"\r\n");
		
	}
	else
	{
		
		if((0X03 == Read_from_AM2320Data[0]) && (0X04 == Read_from_AM2320Data[1]))
		{
			gnAM2320_humidi = ((Read_from_AM2320Data[2]<<8) | Read_from_AM2320Data[3]);
			//stprintf(ID_DEBUG,"湿度：:  ");
			//nprintf(ID_DEBUG,gnAM2320_humidi,0,DEC);
			//stprintf(ID_DEBUG,"\r\n");
			
			gnAM2320_temper = ((Read_from_AM2320Data[4]<<8) | Read_from_AM2320Data[5]);

			//判断温度是否是负值
			if(gnAM2320_temper >= 0)
			{
				gnAM2320_temper = gnAM2320_temper;
			}
			else
			{
				gnAM2320_temper = (-1) * gnAM2320_temper;
			}
			
			//stprintf(ID_DEBUG,"温度：:  ");
			//nprintf(ID_DEBUG,gnAM2320_temper,0,DEC);
			//stprintf(ID_DEBUG,"\r\n");
			
		}
		else
		{

			stprintf(ID_DEBUG,"IIC is Err ");
		
		}
		
	}
	//OSTimeDly(OS_TICKS_PER_SEC);
	//Delay(10000000);
	
}


//获取温度值
short AM2320_get_temper(void)
{
    return gnAM2320_temper;
}

//获取湿度值
short AM2320_get_humidi(void)
{
    return gnAM2320_humidi;
}






