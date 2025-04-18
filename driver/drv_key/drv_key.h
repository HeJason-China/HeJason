#ifndef __DRV_KEY_H
#define __DRV_KEY_H

#include "stm32f4xx.h"

#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)	//PE5 
#define KEY3 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) //PE6
#define KEY4        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) //PC13

struct keys{
	uint8_t age;
	uint8_t flag;
};

void drv_KEY_Init(void);
void key_serv(void);


#endif
