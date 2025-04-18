#include "drv_key.h"
#include "drv_systick.h"

extern uint8_t ui;    
struct keys key[5] = {0,0};

void drv_KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

uint8_t key_read()
{
	if(KEY1==RESET) return 1;
	else if(KEY2==RESET) return 2;
	else if(KEY3==RESET) return 3;
	else if(KEY4==RESET) return 4;
	else return 0;
}

void key_serv()
{
	uint8_t key_sta = key_read();
	if(key_sta != 0)
	{
		key[key_sta].age ++;
		if(key[key_sta].age == 2)
			key[key_sta].flag = 1;
	}
	else
	{
		for(int i=0;i<5;i++) key[i].age = 0;
	}
}

