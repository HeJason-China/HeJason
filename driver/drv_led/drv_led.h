#ifndef __DRV_LED_H
#define __DRV_LED_H

#include "stm32f4xx.h"

void drv_LED_Init(void);

void LED_OFF(void);
void FAN_ON(void);
void FAN_OFF(void);
void BEEP_ON(void);
void BEEP_OFF(void);
void BEEP_Circle(void);
void LED3_ON(void);
void LED3_OFF(void);
#endif
