#include  "BSP_AM2320.h"

/**
  ******************************************************************************
  * @file    BSP_AM2320.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   AM2320传感器驱动
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
  * @brief  AM2320传感器初始化函数
  * @param  无
  * @retval 无
*/
void bsp_AM2320_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(AM2320_IO_SDA_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; //不上拉不下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	
}

/*
 * 函数名：AM2320_Mode_IPU
 * 描述  ：使AM2320-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void AM2320_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*选择要控制的DHT11_PORT引脚*/	
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;

	/*设置引脚模式为浮空输入模式*/ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ; 

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_PORT*/
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	 
}

/*
 * 函数名：AM2320_Mode_Out_PP
 * 描述  ：使AM2320-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void AM2320_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*选择要控制的DHT11_PORT引脚*/															   
	GPIO_InitStructure.GPIO_Pin = AM2320_IO_SDA_PIN;	

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*设置引脚的输出类型为推挽输出*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_PORT*/
	GPIO_Init(AM2320_IO_SDA_GPIO_PORT, &GPIO_InitStructure);	 	 
}
/* 
 * 从DHT11读取一个字节，MSB先行
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(AM2320_DATA_IN()==Bit_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		delay_us(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(AM2320_DATA_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(AM2320_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}
/*
 * 一次完整的数据传输为40bit，高位先出
 * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 *	返回 1  错误  0正常
 */
uint8_t bsp_AM2320_Read_Datas(AM2320_Data_TypeDef *AM2320_Data)
{ 
	uint8_t  humi_High;		//湿度的整数部分
	uint8_t  humi_Low;	 	//湿度的小数部分
	uint8_t  temp_High;	 	//温度的整数部分
	uint8_t  temp_Low;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和	
  uint16_t count;
	/*输出模式*/
	AM2320_Mode_Out_PP();
	/*主机拉低*/
	AM2320_DATA_OUT(AM2320_LOW);
	/*延时18ms*/
	delay_us(2000);

	/*总线拉高 主机延时30us*/
	AM2320_DATA_OUT(AM2320_HIGH); 

	delay_us(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	AM2320_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(AM2320_DATA_IN()==Bit_RESET)     
	{
    count=0;
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(AM2320_DATA_IN()==Bit_RESET)
    {
      count++;
      if(count>1000)  return 0;
      delay_us(10); 
    }    
    
    count=0;
		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(AM2320_DATA_IN()==Bit_SET)
    {
      count++;
      if(count>1000)  return 0;
      delay_us(10); 
    }  
		/*开始接收数据*/   
		humi_High=  Read_Byte();

		humi_Low= Read_Byte();

		temp_High=  Read_Byte();

		temp_Low= Read_Byte();

		check_sum= Read_Byte();

		/*读取结束，引脚改为输出模式*/
		AM2320_Mode_Out_PP();
		/*主机拉高*/
		AM2320_DATA_OUT(AM2320_HIGH);
		/*检查读取的数据是否正确*/
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
