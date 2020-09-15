
#ifndef 	_BSP_LORA_H_
#define 	_BSP_LORA_H_

#define RX_LENGTH  254

void USART1_IRQHandler(void);
extern void bsp_Lora_init(void);
extern void uart1_send(uint8 *sp,uint16 len) ;


#endif

