
#ifndef 	_BSP_RF_H_
#define 	_BSP_RF_H_


#define  CPU_IntDis()           { __set_PRIMASK(1); }	/* ���ж� */
#define  CPU_IntEn()            { __set_PRIMASK(0); }	/* ���ж� */

#define RX_LENGTH  254


void bsp_Debug_Init(void);

void UART4_IRQHandler(void);
extern void UART4_Config(void);
extern void uart4_send(uint8 *sp,uint16 len) ;

#endif
