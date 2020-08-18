#ifndef _AM2320_BY_IIC_H_
#define _AM2320_BY_IIC_H_

/************************************************* 
*Function:		AM2320_and_iic_init
*Input:			
*OUTPUT:		void
*Return:		
*DESCRIPTION:  AM2320 芯片状态初始化 以及系统初始化
*************************************************/
void AM2320_and_iic_init(void);

void AM2320_Get_Data(void);


//获取温度值
short AM2320_get_temper(void);

//获取湿度值
short AM2320_get_humidi(void);


//I2C初始化信息结构体
typedef struct
{
    unsigned char controlMode;  ///<I2C控制方式
    unsigned int  speed;        ///<I2C读写速度
    unsigned char subAddrMode;  ///<是否有子地址
    unsigned char subAddrWidth; ///<子地址字节数
    unsigned short slaveAddr;   ///<设备地址
	unsigned char channel;		///<需要初始化的通道
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
