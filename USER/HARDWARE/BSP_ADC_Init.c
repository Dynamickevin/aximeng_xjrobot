#include  "BSP_ADC_Init.h"

/**
  ******************************************************************************
  * @file    BSP_ADC_Init.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   ADC��ʼ��
  ******************************************************************************
  * @attention
  *			
  *
  ******************************************************************************
  */
__IO uint16_t ADC_Value[ADC_BUFFSIZE];
 /*
  * @brief  ADC��ʼ������
  * @param  ��
  * @retval ��
*/
void bsp_ADC_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	// ʹ�� GPIO ʱ��
 // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
	RCC_AHB1PeriphClockCmd(ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_ADDR;	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Value;  
	// ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFSIZE;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	// // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	//	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
	// FIFO ��С��FIFOģʽ��ֹʱ�������������	
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
	DMA_InitStructure.DMA_Channel = ADC_DMA_CHANNEL; 
	//��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
	DMA_Cmd(ADC_DMA_STREAM, ENABLE);

	RCC_APB2PeriphClockCmd(ADC_CLK , ENABLE);
	// -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	// ʱ��Ϊfpclk x��Ƶ	
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	// ��ֹDMAֱ�ӷ���ģʽ	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// ����ʱ����	
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
	ADC_CommonInit(&ADC_CommonInitStructure);

	// -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
	// ADC �ֱ���
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	// ɨ��ģʽ����ͨ���ɼ���Ҫ	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
	// ����ת��	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	//��ֹ�ⲿ���ش���
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	//�����Ҷ���	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//ת��ͨ�� 4��
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

