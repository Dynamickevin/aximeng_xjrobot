

#ifndef _USART_API_H_
#define _USART_API_H_

#define RX_LENGTH  254


void USART1_IRQHandler(void);
extern void USART1_Config(void);
extern void uart1_send(uint8 *sp,uint16 len) ;

void USART2_IRQHandler(void);
extern void USART2_Config(void);
extern void uart2_send(uint8 *sp,uint16 len) ;

void USART3_IRQHandler(void);
extern void USART3_Config(void);
extern void uart3_send(uint8 *sp,uint16 len) ;


//void UART5_IRQHandler(void);
//extern void UART5_Config(void);
//extern void uart5_send(uint8 *sp,uint8 len) ;


#endif
