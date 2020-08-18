#ifndef _AM2320_BY_IIC_H_
#define _AM2320_BY_IIC_H_

/************************************************* 
*Function:		AM2320_and_iic_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  AM2320 оƬ״̬��ʼ�� �Լ�ϵͳ��ʼ��
*************************************************/
void AM2320_and_iic_init(void);

void AM2320_Get_Data(void);


//��ȡ�¶�ֵ
short AM2320_get_temper(void);

//��ȡʪ��ֵ
short AM2320_get_humidi(void);


//I2C��ʼ����Ϣ�ṹ��
typedef struct
{
    unsigned char controlMode;  ///<I2C���Ʒ�ʽ
    unsigned int  speed;        ///<I2C��д�ٶ�
    unsigned char subAddrMode;  ///<�Ƿ����ӵ�ַ
    unsigned char subAddrWidth; ///<�ӵ�ַ�ֽ���
    unsigned short slaveAddr;   ///<�豸��ַ
	unsigned char channel;		///<��Ҫ��ʼ����ͨ��
}I2C_INIT_INFO;

#define MASTER_MODE_SELECT_FAILED                          (1)
#define MASTER_TRANSMITTER_MODE_SELECTED_FAILED            (2)    
#define MASTER_RECEIVER_MODE_SELECTED_FAILED               (3)
#define MASTER_BYTE_RECEIVED_FAILED                        (4)
#define MASTER_BYTE_TRANSMITTING_FAILED                    (5)
#define MASTER_BYTE_TRANSMITTED_FAILED                     (6)
#define MASTER_MODE_ADDRESS10_FAILED                       (7)
#define MASTER_GET_I2C_FLAG_BUSY_FAILED					   (8)

extern uint8 VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info);
extern uint8 VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32 subaddr, uint8 *s, uint32 num);
extern uint8 VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32 suba,uint8 *s,uint32 num);


#endif
