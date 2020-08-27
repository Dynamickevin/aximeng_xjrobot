#include  "BSP_ADC_Init.h"

/**
  ******************************************************************************
  * @file    BSP_ADC_Init.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   ADC初始化
  ******************************************************************************
  * @attention
  *			
  *
  ******************************************************************************
  */
__IO uint16_t ADC_Value[ADC_BUFFSIZE];
 /*
  * @brief  ADC初始化函数
  * @param  无
  * @retval 无
*/
void bsp_ADC_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	// 使能 GPIO 时钟
 // ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
	RCC_AHB1PeriphClockCmd(ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_ADDR;	
	// 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Value;  
	// 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFSIZE;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	// // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	//	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 禁止DMA FIFO	，使用直连模式
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
	// FIFO 大小，FIFO模式禁止时，这个不用配置	
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
	DMA_InitStructure.DMA_Channel = ADC_DMA_CHANNEL; 
	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
	DMA_Cmd(ADC_DMA_STREAM, ENABLE);

	RCC_APB2PeriphClockCmd(ADC_CLK , ENABLE);
	// -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	// 时钟为fpclk x分频	
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	// 禁止DMA直接访问模式	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// 采样时间间隔	
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
	ADC_CommonInit(&ADC_CommonInitStructure);

	// -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
	// ADC 分辨率
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	// 扫描模式，多通道采集需要	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
	// 连续转换	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	//禁止外部边沿触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//外部触发通道，本例子使用软件触发，此值随便赋值即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	//数据右对齐	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//转换通道 4个
	ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_NUM;                                    
	ADC_Init(ADC_USE, &ADC_InitStructure);
	//---------------------------------------------------------------------------

	ADC_RegularChannelConfig(ADC_USE, SLAVE_MOTOR_SPEED_ADC_CHANNEL, 1, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC_USE, MASTER_MOTOR_SPEED_ADC_CHANNEL, 2, ADC_SampleTime_56Cycles);
	
	ADC_RegularChannelConfig(ADC_USE, PRESSURE_SENSOR_ADC_CHANNEL, 3, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC_USE, HI3521_NTC_ADC_CHANNEL, 4, ADC_SampleTime_56Cycles);
	
	
	ADC_DMARequestAfterLastTransferCmd(ADC_USE, ENABLE);
	ADC_DMACmd(ADC_USE, ENABLE);
	ADC_Cmd(ADC_USE, ENABLE);  
	ADC_SoftwareStartConv(ADC_USE);

}

