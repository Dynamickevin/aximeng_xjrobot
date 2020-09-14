#include "BSP_GPS.h"
#include <math.h>
#include "string.h"
/************************************************************************/
/*********************GPS��������*****************************/
/**/
/**/
/**/
/**/
/**/
/************************************************************************/


char GPS_F;									
char GGAData_buf[100];
char RMCData_buf[100];
char gps_data[8][15];
char headerData[6];

struct {
	u8 flag;
	char GGAData[100];
	uint8_t GGAData_Len;
}GGAData;
struct {
	u8 flag;
	char RMCData[100];
	uint8_t RMCData_Len;
}RMCData;

USART_TypeDef * GPS_UART;
uint16_t dataMode,headerCount,dataCount,readCount;
u8 GGADataFinish,RMCDataFinish;

double  currentLatitude,currentLongitude;
unsigned char currentSatellitesUsed;
float HDOP;

time currentTime,homeTime,lastTime;
char GPS_Init_Flag = 0;
char gps_data[8][15]={"NULL,","NULL,","NULL,","NULL,","NULL,","NULL,","NULL,","NULL,"};
int TimingDelay;


//�ڲ���������
static void  bsp_GPS_TTL_Init(void);
static void  bsp_GPS_GPS_RS232_Init(void);
void GPS_UART_ISR(u8 dataTemp);



static u8 GetTime(time *time ,char *data);//hh:mm:ss.sss
static u8  GetLatitude (double *latitude, char *data);//ddmm.mmmmm;
static void GetLongitude (double *longitude, char *data);
static void GPS_Get_HDOP(float *HDOP,char *data,u8 size);
 /*
  * @brief  GPS��ʼ��������GPS��ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/
void  bsp_GPS_Init(GPS_PORT  GPS_COM)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd ( GPS_EN_GPIO_CLK, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = GPS_EN_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(GPS_EN_GPIO_PORT, &GPIO_InitStructure);	
	bsp_GPS_cmd(DISABLE);
	
	if(GPS_COM == GPS_TTL)
	{
		bsp_GPS_TTL_Init();
		GPS_UART = GPS_TTL_UART;
	}
	else if(GPS_COM == GPS_RS232)
	{
		GPS_UART = GPS_RS232_UART;
		bsp_GPS_GPS_RS232_Init();
	}
}

 /*
  * @brief  GPS_TTL��ʼ��������GPS_TTL��ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/
static void  bsp_GPS_TTL_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd( GPS_TTL_UART_RX_GPIO_CLK|GPS_TTL_UART_TX_GPIO_CLK, ENABLE);

	/* ʹ�� UART ʱ�� */
	RCC_APB1PeriphClockCmd(GPS_TTL_UART_CLK, ENABLE);

	/* ���� PXx �� USARTx_Tx ���Ÿ���*/ 
	GPIO_PinAFConfig(GPS_TTL_UART_RX_GPIO_PORT,GPS_TTL_UART_RX_SOURCE, GPS_TTL_UART_RX_AF);

	/*  ���� PXx �� USARTx__Rx ���Ÿ���*/
	GPIO_PinAFConfig(GPS_TTL_UART_TX_GPIO_PORT,GPS_TTL_UART_TX_SOURCE,GPS_TTL_UART_TX_AF);

	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPS_TTL_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPS_TTL_UART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPS_TTL_UART_RX_PIN;
	GPIO_Init(GPS_TTL_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* ���ô���GPS_TTL_UART ģʽ */
	USART_InitStructure.USART_BaudRate = GPS_TTL_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(GPS_TTL_UART, &USART_InitStructure); 


	/*���ô��ڽ����ж�*/
	USART_ITConfig(GPS_TTL_UART, USART_IT_RXNE, ENABLE);

	USART_Cmd(GPS_TTL_UART, ENABLE);
}

void GPS_TTL_UART_IRQHandler(void)
{

	u8 tmp;
	if(USART_GetITStatus(GPS_TTL_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(GPS_TTL_UART);
		GPS_UART_ISR(tmp);
	}
}

 /*
  * @brief  GPS_RS232��ʼ��������GPS_RS232��ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/
static void  bsp_GPS_GPS_RS232_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd( GPS_RS232_UART_RX_GPIO_CLK|GPS_RS232_UART_TX_GPIO_CLK, ENABLE);

	/* ʹ�� UART ʱ�� */
	RCC_APB1PeriphClockCmd(GPS_RS232_UART_CLK, ENABLE);

	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPS_RS232_UART_RX_GPIO_PORT,GPS_RS232_UART_RX_SOURCE, GPS_RS232_UART_RX_AF);

	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPS_RS232_UART_TX_GPIO_PORT,GPS_RS232_UART_TX_SOURCE,GPS_RS232_UART_TX_AF);

	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPS_RS232_UART_TX_PIN  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPS_RS232_UART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPS_RS232_UART_RX_PIN;
	GPIO_Init(GPS_RS232_UART_RX_GPIO_PORT, &GPIO_InitStructure);

	/* ���ô���GPS_RS232_UART ģʽ */
	USART_InitStructure.USART_BaudRate = GPS_RS232_UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(GPS_RS232_UART, &USART_InitStructure); 

	/*���ô��ڽ����ж�*/
	USART_ITConfig(GPS_RS232_UART, USART_IT_RXNE, ENABLE);

	USART_Cmd(GPS_RS232_UART, ENABLE);
}

 /*
  * @brief  GPS_RS232 �жϴ�����
  * @param  ��
  * @retval ��
*/
//void GPS_RS232_UART_IRQHandler(void)
//{
//	u8 tmp;
//	if(USART_GetITStatus(GPS_RS232_UART, USART_IT_RXNE) != RESET)
//	{
//		tmp = USART_ReceiveData(GPS_RS232_UART);
//		GPS_UART_ISR(tmp);
//	}
//}


 /*
  * @brief  GPS ��Դ�����ر�����
  * @param  ENABLE������
  *			DISABLE �ر�
  * @retval ��
*/
void bsp_GPS_cmd(FunctionalState NewSta)
{
	if(NewSta == ENABLE)
	{
		GPIO_SetBits(GPS_EN_GPIO_PORT,GPS_EN_PIN);
	}
	else
	{
		GPIO_ResetBits(GPS_EN_GPIO_PORT,GPS_EN_PIN);
	}
}


 /*
  * @brief  GPS �������ݴ�����
  * @param   dataTemp ���յ�������
  * @retval ��
*/
void GPS_UART_ISR(u8 dataTemp)
{
	if( dataTemp == '$' )
	{
		dataMode = 1;
	}
	if(dataTemp == 0X0A)
	{//'*')
		if( dataMode == 2 )
		{	
			GGAData.flag = 1;
			GGAData.GGAData_Len = dataCount;
			GGAData.GGAData[dataCount+1] = 0;
			memcpy(GGAData.GGAData,GGAData_buf,sizeof(GGAData_buf));		
		}
		else if( dataMode == 3)
		{
			RMCData.flag = 1;
			RMCData.RMCData[dataCount+1] = 0;
			memcpy(RMCData.RMCData,RMCData_buf,sizeof(RMCData_buf));
			RMCData.RMCData_Len = dataCount;
		}
		dataMode  = 0;
		//LED_Blink();
	}
	switch(dataMode){
		case 1: 
			headerData[ headerCount ] = dataTemp;
			headerCount ++;
			if(headerCount >= 6){
				if(strncmp ( headerData+1, GGAHeader, 5) == 0){
					dataMode    = 2;
				}else if(strncmp ( headerData+1, RMCHeader, 5) == 0){
					dataMode    = 3;
				}
				else if(strncmp ( headerData+1, GSAHeader, 5) == 0){
					dataMode    = 4;
				}else if(strncmp ( headerData+1, VTGHeader, 5) == 0){
					dataMode    = 4;
				}else{
					dataMode    = 0;	
				}	
				dataCount   = 0;					
				headerCount = 0;
			}
			break;
		case 2:
			GGAData_buf[ dataCount ] = dataTemp;	
			dataCount ++;
			if(dataCount>=100){
				dataCount = 0;
				dataMode = 0;
			}
			break;
		case 3:
			RMCData_buf[ dataCount ] = dataTemp;
			dataCount ++;
			if(dataCount>=100){
				dataCount = 0;
				dataMode = 0;
			}
			break;
		case 4 : headerCount = 0;
			dataCount   = 0;
			break;
		case 0:
			headerCount = 0;
			dataCount   = 0;
			break;
		default:
			dataMode  = 0;
			break;
	}	 
}



 /*
  * @brief 	γ�ȴ��� ��ת��Ϊ�Ƕ�
  * @param   latitude ת����洢��γ��
  *			 data	��ת��������
  * @retval ��
*/
static u8 GetLatitude (double *latitude, char *data)//ddmm.mmmmm 
{
	double latitudeTemp;
	if(*data == ',')return 1;	
	latitudeTemp =   (double)( data[0] - 0x30 ) * 60000000
					+(double)( data[1] - 0x30 ) * 6000000
					+(double)( data[2] - 0x30 ) * 1000000
					+(double)( data[3] - 0x30 ) * 100000
					+(double)( data[5] - 0x30 ) * 10000
					+(double)( data[6] - 0x30 ) * 1000	
					+(double)( data[7] - 0x30 ) * 100
					+(double)( data[8] - 0x30 ) * 10;
					//+(double)( data[9] - 0x30 ) * 1;	
	*latitude=	latitudeTemp / 6000000;
	return 0;
}

 /*
  * @brief 	���ȴ��� ��ת��Ϊ�Ƕ�
  * @param   longitude ת����洢�ľ���
  *			 data	��ת��������
  * @retval ��
*/
static void GetLongitude (double *longitude, char *data)
{
	double longitudeTemp;
	longitudeTemp = (double)( data[0] - 0x30 ) * 600000000
					+(double)( data[1] - 0x30 ) * 60000000
					+(double)( data[2] - 0x30 ) * 6000000
					+(double)( data[3] - 0x30 ) * 1000000
					+(double)( data[4] - 0x30 ) * 100000
					+(double)( data[6] - 0x30 ) * 10000	
					+(double)( data[7] - 0x30 ) * 1000
					+(double)( data[8] - 0x30 ) * 100
					+(double)( data[9] - 0x30 ) * 10;
					//+(double)( data[10] - 0x30 ) * 1;
	*longitude=	longitudeTemp / 6000000;				
}
 /*
  * @brief 	ʱ�䴦�� 
  * @param   time    ת����洢��ʱ��
  *			 data	 ��ת��������
  * @retval ��
*/
static u8 GetTime(time *time ,char *data)
{
	u8 i;
	u8 buf[6];
	for(i = 0 ; i < 6;i++)
	{
		buf[i ] = data[i];
		if(buf[i] < '0' || buf[i] > '9')return 1;
	}
	time->h =  ( buf[0] - 0x30 ) * 10 + ( buf[1] - 0x30 );
	time->m =  ( buf[2] - 0x30 ) * 10 + ( buf[3] - 0x30 );
	time->s =  ( buf[4] - 0x30 ) * 10 + ( buf[5] - 0x30 );	
	return 0;
}


 /*
  * @brief 	ˮƽ�������� 
  * @param   HDOP    ת����洢��ˮƽ��������
  *			 data	 ��ת��������
  * @retval ��
*/
void GPS_Get_HDOP(float *HDOP,char *data,u8 size)
{
	float Tmp = 0,Tmp2 = 1;
	u8 flag = 0,i = 0;
	for(i = 0 ; i < size;i++){
		if(data[i] != '.'){
			Tmp= Tmp*10 +data[i]-'0';
			if(flag) Tmp2*=10;
		}else{
			flag = 1;
		}	
	}
	*HDOP = Tmp/Tmp2;
}

 /*
  * @brief 	�ٶ� 
  * @param   HDOP    ת����洢��ˮƽ��������
  *			 data	 ��ת��������
  * @retval �ٶ�ֵ	��
*/
void GPS_Get_Speed(float *Speed,char *data,u8 size)
{
	float Tmp = 0,Tmp2 = 1;
	u8 flag = 0,i = 0;
	for(i = 0 ; i < size;i++)
	{
		if(data[i] != '.'){
			Tmp= Tmp*10 +data[i]-'0';
			if(flag) Tmp2*=10;
		}else{
			flag = 1;
		}	
	}
	*Speed = Tmp/Tmp2;
}
 /*
  * @brief 	����GGA�ֶ� 
  * @param   GPS_Info   �洢�Ľ������GPS��Ϣ
  * @retval ��
*/
u8 GPS_Parse_GGA(GPS_Info_Typedef  *GPS_Info)
{
	u8 i,len;
	char *GGA =GGAData.GGAData;
	char *token,*token2;
	token = strchr(GGA,',');
	if(token == NULL)return 1;
	token++;
	for(i = 0 ; i<8 ;i++)
	{
		token2 = strchr(token,',');
		if(token2 == NULL){
			return i + 2;
		}
		len = token2 - token;
		token2++;
		if(len){
			switch(i)
			{
				case 0:
					GetTime(&GPS_Info->NewTime,token);
					break;
				case 1:
					GetLatitude(&GPS_Info->currentLatitude, token);
					break;
				case 2:
					if(*token == 'N')
						GPS_Info->currentLatitude = GPS_Info->currentLatitude;		
					else
						GPS_Info->currentLatitude = -GPS_Info->currentLatitude;					
					break;
				case 3:
					GetLongitude(&GPS_Info->currentLongitude,token);
					break;
				case 4:
					if(*token == 'E')
						GPS_Info->currentLatitude = GPS_Info->currentLatitude ;		
					else
						GPS_Info->currentLatitude = -GPS_Info->currentLatitude ;					
					break;
				case 5:
					GPS_Info->Status = *token - '0';
					break;
				case 6:
					GPS_Info->currentSatellitesUsed =10* (*token - '0') + *(token+1) - '0';
					break;
				case 7:
					GPS_Get_HDOP(&GPS_Info->HDOP,token,len);
					break;
				default:break;				
			}
			token = token2;	
		}
	}
	return 0;
}
 /*
  * @brief 	����RMC�ֶ� 
  * @param   GPS_Info   �洢�Ľ������GPS��Ϣ
  * @retval 0�����ɹ� 	����Ϊ ����ʧ�� λ��
*/
u8 GPS_Parse_RMC(GPS_Info_Typedef  *GPS_Info)
{
	u8 i,len;
	char *RMC =RMCData.RMCData;
	char *token,*token2;
	
	token = strchr(RMC,',');
	if(token == NULL)return 1;
	token++;
	for(i = 0 ; i<8 ;i++){
		token2 = strchr(token,',');
		if(token2 == NULL){
			return i + 2;
		}
		len = token2 - token;
		token2++;
		if(len){
			switch(i){
				case 0:
//					GetTime(&GPS_Info->NewTime,token);
					break;				
				case 1:
//					if( *token =='A')
//						GPS_Info->Status = 1;
//					else{
//						GPS_Info->Status = 0;
//						return 2;
//					}
					break;
				case 2:
//					GetLatitude(&GPS_Info->currentLatitude, token);
					break;
				case 3:
//					if(*token == 'N')
//						GPS_Info->currentLatitude = GPS_Info->currentLatitude;		
//					else
//						GPS_Info->currentLatitude = -GPS_Info->currentLatitude;					
					break;
				case 4:
//					GetLongitude(&GPS_Info->currentLongitude,token);
					break;
				case 5:
//					if(*token == 'E')
//						GPS_Info->currentLatitude = GPS_Info->currentLatitude ;		
//					else
//						GPS_Info->currentLatitude = -GPS_Info->currentLatitude ;					
					break;				
				case 6:
					GPS_Get_Speed(&GPS_Info->Speed,token,len);
					break;
				default:break;				
			}
			token = token2;	
		}
	}
	return 0;
}

 /*
  * @brief 	��ȡGPS_Info 
  * @param   GPS_Info   �洢�Ľ������GPS��Ϣ
  * @retval 0xff δ�յ������ݣ�����ΪGPS_Parse_GGA����ֵ
*/
u8 bsp_GPS_Get_Info(GPS_Info_Typedef  *GPS_Info)
{
	if(GGAData.flag == 1)
	{
		GGAData.flag = 0;
		GPS_Parse_RMC(GPS_Info);
		return GPS_Parse_GGA(GPS_Info);
	}
	else
	{
		return 0xff;
	}
}
/**************************************************END LINE**********************************************/
