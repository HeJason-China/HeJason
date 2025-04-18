#ifndef __DRV_USART_H
#define __DRV_USART_H

#include "stm32f4xx.h"

void drv_USART1_Init(void);
void drv_USART2_Init(void);

void drv_USART_SendStr(USART_TypeDef* USARTx,char *str,int size);
#endif
