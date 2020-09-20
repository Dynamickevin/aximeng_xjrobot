

#ifndef 	_BSP_BRD_H_
#define 	_BSP_BRD_H_

/**
  *
  *		��Դ���ư� 
  *
**/

#include "stm32f4xx.h"
#include  "board.h"

#define  UART_FIFO_NUM     64  //1032
#define  USART_HEAD_1 	0x55
#define  USART_HEAD_2 	0xaa

#define  DATA_MAX   	512

//�������������Դ���ڲ�ͨѶЭ��  �汾1.1.2

#define			UART_CMD_ACK			    			 0x00 					//ͨ��Ӧ��ָ��

#define			UART_GET_STATUS			    		 0x01 					//��ȡ�豸״ָ̬��
#define			UART_REPLY_GET_STATUS			   0x81 					//��ȡ�豸״̬�ظ�ָ��

#define			UART_GET_PROTECT_CMD			   0x02 					//��ȡ��������ָ��
#define			UART_REPLY_GET_PROTECT			 0x82 					//��ȡ���������ظ�ָ��

#define			UART_SET_PW_STATUS			     0x03 					//���Ƶ�Դ״ָ̬��
#define			UART_REPLY_SET_PW						 0x83 					//���Ƶ�Դ״̬�ظ�ָ��

#define			UART_SET_CHARGE_STATUS	     0x04 					//���Ƴ�翪��ָ��

#define			UART_SET_CELL_BALANCE    	 	 0x05 					//����ƽ�⿪��ָ��

/********************************************************/
	

//  ֡��ʽ�ڲ����ݰ� __PACKED���սṹ Ϊ���㴫��
typedef struct __PACKED{
	u8 Cmd1;
	u8 Cmd2;
	u32 Packet_Length;		// Data_LengthΪ���ݳߴ磬Packet Length = Data_Length+16
	u32	Param;
	u32 Crc32;
	u8 Data[DATA_MAX];
}UARTbuf_Typedef;


//֡��ʽ�ڲ����ݰ� 
typedef struct {
	u8 Packet_Length;		//�����Ӱ�ͷ��У��͵������ֽڳ��ȣ�Data_LengthΪ���ݳߴ磬Packet Length = Data_Length + 8
												//5��Checksum + Packet_Length + Cmd + UART_HEAD2 + UART_HEAD1
	u8  Cmd;							//�������ҵ��������
	u8  Data[DATA_MAX];
	u8  Checksum;					//������ͷ���������������ݵİ��ֽڵ����ֵ��ȡ��8λ
	
}Power_UARTbuf_Typedef;

/************************************************************************************/
//���ڽ���BUF
typedef struct{
	u8 		Timeout;
	u8 		Status;
	u16   Index;
	u16		Datalen;
	u8 		data[DATA_MAX+32];
}RX_Buf_Typedef;

/************************************************************************************/

#define GPIO_BAT_CHARGE_EN      GPIOD,GPIO_Pin_1     //���ʹ��
#define GPIO_BREAK_MEN          GPIOE,GPIO_Pin_1     //��բ���أ��͵�ƽʹ��

//���������
#define GPIO_CHK_CHAG_SHORT	 	 GPIOE,GPIO_Pin_4    //����·���
#define GPIO_CHK_CHAG_JOIN	 	 GPIOE,GPIO_Pin_5	   //��������

//���Ŷ���
/*******************************************************/

#define BRD_UART                             USART2
#define BRD_UART_CLK                         RCC_APB1Periph_USART2

#define BRD_UART_BAUDRATE                    57600	

#define BRD_UART_RX_GPIO_PORT                GPIOA
#define BRD_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_RX_PIN                      GPIO_Pin_2
#define BRD_UART_RX_AF                       GPIO_AF_USART2
#define BRD_UART_RX_SOURCE                   GPIO_PinSource2

#define BRD_UART_TX_GPIO_PORT                GPIOA
#define BRD_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BRD_UART_TX_PIN                      GPIO_Pin_3
#define BRD_UART_TX_AF                       GPIO_AF_USART2
#define BRD_UART_TX_SOURCE                   GPIO_PinSource3

#define BRD_UART_IRQHandler                  USART2_IRQHandler
#define BRD_UART_IRQ                 				 USART2_IRQn


typedef struct {
	u16					Total_voltage;  				//�ܵ�ѹ(mV)���޷�����16bit
	int16_t			Electric_current;				//�ܵ���(mA)���з�����16bit
	int16_t			BTemp[2]; 							//2·�¶�x10 ���з��ţ�
	u16 				Monomer_voltage[6]; 		//6 ������ĵ�ѹ��Ϣ(mV)���޷�����16bit
	u8					Sw_Status;							//4 ·������Ϣ����4bit ��λ��1 ����򿪣�0 ����رգ�
	u8          BMS_Protect_Status;			//BMS �������ϱ�־
	u8					Change_MOS_Status;			//��ŵ�MOS ״̬��־,����2bit ��λ��1 ����򿪣�0 ����رգ�
	u8					Battery_Balance_Status; //�Ƿ����õ��ƽ�⹦��,��0x01 ��������ƽ�⣬0x00 ����ر�ƽ�⹦�ܡ�
	int16_t			Accel_Value_X;		//X �᷽����ٶ�ֵ����λΪ0.1m/s2
	int16_t			Accel_Value_Y;		//Y �᷽����ٶ�ֵ����λΪ0.1m/s2
	int16_t			Accel_Value_Z;		//Z �᷽����ٶ�ֵ����λΪ0.1m/s2
} BRD_Info_Typedef;


typedef struct {
	u8		Short_Protect_Current_threshold;  					//��·������������(A)���޷�����8bit
	u8		Overload_Protect_Current_threshold;					//���ر�����������(A)���޷�����8bit
	u16		Monomer_overvoltage_Protect_threshold; 			//�����ѹ��������(mV)���޷�����16bit
	u16 	Monomer_Undervoltage_Protect_threshold; 		//����Ƿѹ��������(mV)���޷�����16bit
	u16		Discharge_capacity_Protect_threshold;				//�ŵ�������������(mAh)���޷�����16bit
} BAT_Protect_Info_Typedef;

typedef struct {
	u8		Power_SW_Status;  					//4 ·���ؿ��ƣ���4bit ��λ��1 ����򿪣�0 ����رգ�
	u8		Mst_Brake_Status_Flag;			//�������բ״̬
	u8		Motor_Power_Status_Flag;		//�����Դ״̬
	u8		Cam_Power_24V_Status_Flag;	//����ͷ24V��Դ״̬
	u8		Cam_Power_12V_Status_Flag;	//����ͷ12V��Դ״̬
} Power_Control_Status_Typedef;

typedef struct {
	u8		Bat_Charge_Enable_Flag;  					//Data[0]: 1 ����򿪳�繦�ܣ�0 ����رճ�繦��
} Power_Charge_Status_Typedef;

typedef struct {
	u8		Cell_Balance_Enable_Flag;  					//��Data[0]: 0x01 ����򿪵���ƽ�⹦�ܣ�0x00 ����رյ���ƽ�⹦�ܡ�ϵͳ������Ĭ�Ϲر�״̬							
} Power_Cell_Balance_Status_Typedef;

extern 	BRD_Info_Typedef 								BRD_Info;
extern	BAT_Protect_Info_Typedef				BAT_Protect_Info;
extern  Power_Control_Status_Typedef 		Power_Control_Status; 
extern	Power_Charge_Status_Typedef     Power_Charge_Status;
extern	Power_Cell_Balance_Status_Typedef  Power_Cell_Balance_Status;

extern BRD_Info_Typedef gBattery_Value;

extern UARTbuf_Typedef UART_Rx_Buf;
extern Power_UARTbuf_Typedef Power_UART_Rx_Buf;

void UART_Send_Cmd(USART_TypeDef * UART , u16 CMD ,u32 Data_Length,u32 Param,u8 *data);
void UART_Tx_Byte(USART_TypeDef* USARTx, uint8_t Data);
 
void  bsp_BRD_Init(void);

void bsp_Power_UART_TX(u8 Cmd,u8 Datalen,u8*data);

void Send_USART_CMD(u8 cmd, u8* databuf, u8 length);

void USART_CMD_Judgement(void);

void STM_GET_POWER_Info(void);

short POWER_get_BAT_VOL(void);

short POWER_get_BAT_CUR(void);

#endif

