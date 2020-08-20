
#include <includes.h>

short int gnAM2320_temper = 0;
short int gnAM2320_humidi = 0;




I2C_INIT_INFO I2C_Info;	///<I2C�����������Ϣ

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Ӳ��I2C��ʼ�� ��
  * @param  I2C_Info I2C��ʼ����Ϣ��
  * @retval ��ʼ������״̬��
  */
uint8 VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info)
{
	I2C_InitTypeDef  I2C_InitStructure;
   	GPIO_InitTypeDef  GPIO_InitStructure;
   	if(0 == I2C_Info->channel)
	{
			//  I2C3_SCL:PA8 I2C3_SDA��PC9;
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
  * @brief  ͨ��Ӳ��I2Cд���� ��
  * @param  I2Cx Ӳ��I2Cͨ����
  * @param  subaddr д���ݵ��ӵ�ַ�����������ӵ�ַ������������塣
  * @param  s ��Ҫд�����ݻ�������ַ��
  * @param  num ��Ҫд�������ֽ�����
  * @retval д����״̬��
  */
uint8 VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num)
{
  	uint32_t subaddrNum=I2C_Info.subAddrWidth;
	uint32_t retry=0;
	uint8_t *p_data=s;
	I2C_GenerateSTART(I2Cx, ENABLE);   //����һ����ʼ�ź�
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
	//�����豸��ַ
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
				I2C_GenerateSTOP(I2Cx, ENABLE);   //����һ��ֹͣ�ź�
				return MASTER_TRANSMITTER_MODE_SELECTED_FAILED;	
			}
		}
	}
  	
	//�����ӵ�ַ
	while(subaddrNum>0)
	{
	  	I2C_SendData(I2Cx, subaddr>>((subaddrNum-1)*8));
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);		//����һ��ֹͣ�ź�
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
		subaddrNum--;
	}
	//��������
	while(num--)
	{
	  	I2C_SendData(I2Cx, *p_data++); 
		retry=0;
  		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000)
			{
				I2C_GenerateSTOP(I2Cx, ENABLE);			//����һ��ֹͣ�ź�
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
	}
	//ֹͣ����
	I2C_GenerateSTOP(I2Cx, ENABLE);
	return 0;
}
/**
  * @brief  ͨ��Ӳ��I2C������ ��
  * @param  I2Cx Ӳ��I2Cͨ����
  * @param  subaddr �����ݵ��ӵ�ַ�����������ӵ�ַ������������塣
  * @param  s ��Ҫ�������ݴ洢��������ַ��
  * @param  num ��Ҫ���������ֽ�����
  * @retval ������״̬��
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
		//�����豸��ַ
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
		//�����ӵ�ַ
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
		//��������I2C����
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
		//���Ͷ��豸��ַ
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
	//��ȡ����
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
  * @brief  ��ʱ����
  * @param  Time ��ʱʱ�����
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
*DESCRIPTION:  AM2320 оƬ״̬��ʼ�� �Լ�ϵͳ��ʼ��
*************************************************/

extern I2C_INIT_INFO I2C_Info;	///<I2C�����������Ϣ


void AM2320_and_iic_init(void)
{

	#define AM2320_ADDRESS 0xB8
	
	//uint8 AM2320_TX_Buffer[] = {0x03,0x00,0x04};//����ʪ������(��CRCУ��)
	//uint8 AM2320_RX_Buffer[12];
	
	//����AM2320����������I2C��ʼ������
	I2C_Info.controlMode = 1;		//Ӳ������
	I2C_Info.speed=100000;			//100K
	I2C_Info.subAddrMode=2;			//���ӵ�ַģʽ
	I2C_Info.subAddrWidth=0;		//�ӵ�ַΪ0�ֽڿ��
	I2C_Info.slaveAddr=AM2320_ADDRESS;		//AM2320������I2C��ַ
	I2C_Info.channel=0;				//ʹ��I2C3
	VT_I2C_HardInit(&I2C_Info);
		
}


void AM2320_Get_Data(void)
{

	uint8		ret;
	uint8   AM2320Wake[2] ;
	uint8   Send_to_AM2320Data[8];
	uint8   Read_from_AM2320Data[10];
	
	AM2320Wake[0] = 0x00;
	VT_I2C_HardWriteNByte(I2C3,0,AM2320Wake,1);//��㷢��һ�����ݳ�ȥ������������˯���л���
	//OSTimeDly(1);
		
	Send_to_AM2320Data[0]=0x03;
	Send_to_AM2320Data[1]=0x00;
	Send_to_AM2320Data[2]=0x04;
	ret = VT_I2C_HardWriteNByte(I2C3,0,Send_to_AM2320Data,3);//���Ͷ�ȡ�¶Ⱥ�ʪ�ȵ�ָ��
	if(ret)
	{
		stprintf(ID_DEBUG,"I2Cд���ݴ���:  ");
		nprintf(ID_DEBUG,ret,0,DEC);
		stprintf(ID_DEBUG,"\r\n");
		
	}
	OSTimeDly(2);
	//Delay(100000);//����������ʱ�����򴫸���û׼�������ݻ���ֶ����ݴ���
	//GpioSetL(GPIO_LED_SHOW1);
	ret = VT_I2C_HardReadNByte(I2C3,0,Read_from_AM2320Data,8);//��ȡ�¶Ⱥ�ʪ��
	if(ret)
	{
		stprintf(ID_DEBUG,"I2Cд���ݴ���:  ");
		nprintf(ID_DEBUG,ret,0,DEC);
		stprintf(ID_DEBUG,"\r\n");
		
	}
	else
	{
		
		if((0X03 == Read_from_AM2320Data[0]) && (0X04 == Read_from_AM2320Data[1]))
		{
			gnAM2320_humidi = ((Read_from_AM2320Data[2]<<8) | Read_from_AM2320Data[3]);
			//stprintf(ID_DEBUG,"ʪ�ȣ�:  ");
			//nprintf(ID_DEBUG,gnAM2320_humidi,0,DEC);
			//stprintf(ID_DEBUG,"\r\n");
			
			gnAM2320_temper = ((Read_from_AM2320Data[4]<<8) | Read_from_AM2320Data[5]);

			//�ж��¶��Ƿ��Ǹ�ֵ
			if(gnAM2320_temper >= 0)
			{
				gnAM2320_temper = gnAM2320_temper;
			}
			else
			{
				gnAM2320_temper = (-1) * gnAM2320_temper;
			}
			
			//stprintf(ID_DEBUG,"�¶ȣ�:  ");
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


//��ȡ�¶�ֵ
short AM2320_get_temper(void)
{
    return gnAM2320_temper;
}

//��ȡʪ��ֵ
short AM2320_get_humidi(void)
{
    return gnAM2320_humidi;
}






