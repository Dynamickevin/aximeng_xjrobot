#ifndef _GPIO_DEFINE_H__
#define _GPIO_DEFINE_H__

//#define GPIO_MOTOR_PWR_EN       GPIOD,GPIO_Pin_0     //�����Դʹ��  
//#define GPIO_BAT_CHARGE_EN      GPIOD,GPIO_Pin_1     //���ʹ��
//#define GPIO_BREAK_MEN          GPIOE,GPIO_Pin_1     //��բ���أ��͵�ƽʹ��

//#define GPIO_4G_EN        	    GPIOA,GPIO_Pin_8     //4G���أ��͵�ƽʹ��
//#define GPIO_GPS_EN        		  GPIOC,GPIO_Pin_8    //GPS���أ��͵�ƽʹ��
//#define GPIO_12V_EN        		  GPIOE,GPIO_Pin_12    //12V���أ��͵�ƽʹ��

//#define GPIO_CAM_AV_EN          GPIOE,GPIO_Pin_9     //ͼ�����أ��͵�ƽʹ��
//#define GPIO_CAM_EN        		  GPIOA,GPIO_Pin_15    //����ͷ���أ��͵�ƽʹ��
//#define GPIO_CTR_CAM_AV_SW 	 	  GPIOE,GPIO_Pin_2     //����ͷͼ���ź��л�

//#define GPIO_CHK_PRS_VOL1        GPIOC,GPIO_Pin_2     //ѹ��ֵADC2
//#define GPIO_CHK_PRS_VOL2        GPIOC,GPIO_Pin_2     //ѹ��ֵADC2

//#define GPIO_CHK_MT_SPD_M        GPIOC,GPIO_Pin_0     //�������ٶ�ADC0
//#define GPIO_CHK_MT_SPD_S     	 GPIOC,GPIO_Pin_1     //�Ӷ����ٶ�ADC1

//#define GPIO_CHK_BAT_CUR         GPIOC,GPIO_Pin_4     //��ص��� ADC14
//#define GPIO_CHK_BAT_VOL	     GPIOC,GPIO_Pin_5     //��ص�ѹ ADC15
//#define GPIO_CHK_GPS_DATA	     GPIOD,GPIO_Pin_2     //GPS���ݽ���
//#define GPIO_CHK_HOARE1	 	 	 GPIOE,GPIO_Pin_7	  //�������1
//#define GPIO_CHK_HOARE2 	 	 GPIOE,GPIO_Pin_8	  //�������2

//#define GPIO_CHK_CHAG_SHORT	 	 GPIOE,GPIO_Pin_4     //����·���
//#define GPIO_CHK_CHAG_JOIN	 	 GPIOE,GPIO_Pin_5	  //��������
//#define GPIO_CHK_CHAOXUE1	 	 GPIOD,GPIO_Pin_15	  //���볲Ѩ���ؼ��1
//#define GPIO_CHK_CHAOXUE2	 	 GPIOD,GPIO_Pin_14	  //���볲Ѩ���ؼ��2
//#define GPIO_CHK_LIMIT1   		 GPIOB,GPIO_Pin_0     //�Ӷ����˶�������λ���ؼ��1
//#define GPIO_CHK_LIMIT2   		 GPIOB,GPIO_Pin_1     //�Ӷ����˶�������λ���ؼ��2


//#define GPIO_CTL_FED_WTDG 	 	 GPIOC,GPIO_Pin_0	  //���Ź�ι��
#define GPIO_CTL_ARM_RST 	 	 	 GPIOA,GPIO_Pin_11	  //ARM��λ���ƽ�
#define GPIO_CTR_RF433_M0 	 	 GPIOE,GPIO_Pin_13	  //���ߴ���ģʽѡ��0
#define GPIO_CTR_RF433_M1 	 	 GPIOE,GPIO_Pin_14	  //���ߴ���ģʽѡ��1
#define GPIO_CTR_RF433_AUX 	 	 GPIOE,GPIO_Pin_15    //���ߴ��ڼ��
//#define GPIO_CTR_FLASH_CS 	 	 GPIOD,GPIO_Pin_8	  //FLASHƬѡ

//#define GPIO_LED_SHOW2          GPIOD,GPIO_Pin_4      //��·����ָʾ��2
//#define GPIO_LED_SHOW1          GPIOD,GPIO_Pin_7      //��·����ָʾ��1
//#define GPIO_LED_OUT_SHOW1      GPIOD,GPIO_Pin_9      //���ָʾ�� 1
//#define GPIO_LED_OUT_SHOW2      GPIOD,GPIO_Pin_10     //���ָʾ�� 2

//#define GPIO_MTS_CODEA1 	 	 GPIOA,GPIO_Pin_15    //�ӵ�������� TIM2-CH1
//#define GPIO_MTS_CODEB1 	 	 GPIOB,GPIO_Pin_3     //�ӵ�������� TIM2-CH2
//#define GPIO_MTS_CODEA2 	 	 GPIOC,GPIO_Pin_6     //�ӵ�������� TIM8-CH1
//#define GPIO_MTS_CODEB2 	 	 GPIOC,GPIO_Pin_7     //�ӵ�������� 

//#define GPIO_MTM_CODEA 	 	   GPIOB,GPIO_Pin_4     //����������� TIM3-CH1
//#define GPIO_MTM_CODEB 	 	   GPIOB,GPIO_Pin_5     //����������� TIM3-CH2

//#define GPIO_MTM_PWM1 	 	  	 GPIOB,GPIO_Pin_14     //�����PWM TIM1-CH2
//#define GPIO_MTM_PWM2 	 	  	 GPIOB,GPIO_Pin_15     //�����PWM TIM8-CH3

//#define GPIO_CODEA_CD1 	 	  	 GPIOD,GPIO_Pin_12    //�����ӱ����� TIM4-CH1
//#define GPIO_CODEA_CD2	 	  	 GPIOD,GPIO_Pin_13    //�����ӱ����� TIM4-CH2

//#define GPIO_IIC_SCL 	 	  	 GPIOB,GPIO_Pin_6     //IIC ʱ��
//#define GPIO_IIC_SDA 	 	  	 GPIOB,GPIO_Pin_7     //IIC ����

//#define GPIO_SPI_SCK 	 	  	 GPIOB,GPIO_Pin_13    //SPI2 ʱ��
//#define GPIO_SPI_MISO 	 	  	 GPIOB,GPIO_Pin_14    //SPI2 MISO
//#define GPIO_SPI_MOSI 	 	  	 GPIOB,GPIO_Pin_15    //SPI2 MOSI

//#define GPIO_STM_SWIO 	 	     GPIOA,GPIO_Pin_13	  //SW IO
//#define GPIO_STM_SWCK	 	    	 GPIOA,GPIO_Pin_14    //SW ʱ��

//#define GPIO_OUT_SPARE1 	 	 GPIOC,GPIO_Pin_2	  //�������1
//#define GPIO_OUT_SPARE2 	 	 GPIOC,GPIO_Pin_3	  //�������2

#endif
