
#include "NVIC_IRQ.h"


void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ

		//�ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = LIMIT_SW1_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LIMIT_SW1_PreemptionPriority;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = LIMIT_SW1_SubPriority;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = LIMIT_SW2_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LIMIT_SW2_PreemptionPriority;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = LIMIT_SW2_SubPriority;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = NEST_SW_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NEST_SW_PreemptionPriority;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NEST_SW_SubPriority;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
		  /* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = GPS_TTL_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPS_TTL_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = GPS_TTL_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
			  /* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = GPS_RS232_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GPS_TTL_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = GPS_TTL_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
				  /* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = HI3521_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = HI3521_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = HI3521_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
				  /* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = BRD_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BRD_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = BRD_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
					  /* �����ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = RF_UART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RF_UART_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RF_UART_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ENCCODER_AB_TIM_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ENCCODER_AB_TIM_PreemptionPriority;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ENCCODER_AB_TIM_SubPriority;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
				//�ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = ENCCODER_Z_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ENCCODER_Z_PreemptionPriority;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ENCCODER_Z_SubPriority;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = GENERAL_TIM_PreemptionPriority;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = GENERAL_TIM_SubPriority;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

