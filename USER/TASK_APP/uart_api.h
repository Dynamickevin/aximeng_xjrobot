

#ifndef _USART_API_H_
#define _USART_API_H_

#define RX_LENGTH  254


void USART1_IRQHandler(void);
extern void bsp_Lora_init(void);
extern void uart1_send(uint8 *sp,uint16 len) ;


#endif
