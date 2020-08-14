#include  "BSP_AM2320.h"

/**
  ******************************************************************************
  * @file    BSP_AM2320.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   AM2320����������
  ******************************************************************************
  * @attention
  *		
  * 	
  *		
  *		
  *		
  *
  ******************************************************************************
  */

 /*
  * @brief  AM2320��������ʼ������
  * @param  ��
  * @retval ��
*/
void bsp_AM2320_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(AM2320_IO_SDA_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; //������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	
}

/*
 * ��������AM2320_Mode_IPU
 * ����  ��ʹAM2320-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void AM2320_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/	
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;

	/*��������ģʽΪ��������ģʽ*/ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ; 

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	 
}

/*
 * ��������AM2320_Mode_Out_PP
 * ����  ��ʹAM2320-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void AM2320_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/															   
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;	

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	 	 
}
/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(AM2320_DATA_IN()==Bit_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		delay_us(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

		if(AM2320_DATA_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(AM2320_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}
/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 *	���� 1  ����  0����
 */
uint8_t bsp_AM2320_Read_Datas(AM2320_Data_TypeDef *AM2320_Data)
{ 
	uint8_t  humi_High;		//ʪ�ȵ���������
	uint8_t  humi_Low;	 	//ʪ�ȵ�С������
	uint8_t  temp_High;	 	//�¶ȵ���������
	uint8_t  temp_Low;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���	
  uint16_t count;
	/*���ģʽ*/
	AM2320_Mode_Out_PP();
	/*��������*/
	AM2320_DATA_OUT(AM2320_LOW);
	/*��ʱ18ms*/
	delay_us(2000);

	/*�������� ������ʱ30us*/
	AM2320_DATA_OUT(AM2320_HIGH); 

	delay_us(30);   //��ʱ30us

	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	AM2320_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(AM2320_DATA_IN()==Bit_RESET)     
	{
    count=0;
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(AM2320_DATA_IN()==Bit_RESET)
    {
      count++;
      if(count>1000)  return 0;
      delay_us(10); 
    }    
    
    count=0;
		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(AM2320_DATA_IN()==Bit_SET)
    {
      count++;
      if(count>1000)  return 0;
      delay_us(10); 
    }  
		/*��ʼ��������*/   
		humi_High=  Read_Byte();

		humi_Low= Read_Byte();

		temp_High=  Read_Byte();

		temp_Low= Read_Byte();

		check_sum= Read_Byte();

		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		AM2320_Mode_Out_PP();
		/*��������*/
		AM2320_DATA_OUT(AM2320_HIGH);
		/*����ȡ�������Ƿ���ȷ*/
		if(check_sum == ((humi_High + humi_Low + temp_High+ temp_Low)&0xff)){
			AM2320_Data->humi =(float)(((u16)humi_High<<8)+humi_Low)/10;
			AM2320_Data->temp = (float)(((u16)temp_High << 8)+temp_Low)/10;
			
			return 0;
		}else 
			return 1;
	}
	else
	{		
		return 1;
	}   
}
