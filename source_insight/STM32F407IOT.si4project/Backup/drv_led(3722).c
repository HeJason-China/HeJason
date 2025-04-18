#include "drv_led.h"
#include "drv_systick.h"

void drv_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	LED_OFF();
#if 0
	//开GPIOE的时钟
	RCC->AHB1ENR |= 0x01<<4;
	//开GPIOB的时钟
	RCC->AHB1ENR |= 0x01<<1;
	
	GPIOE->MODER &= ~(0x03<<16);
	GPIOE->MODER |= 0x01<<16;
	GPIOE->ODR |= 0x01<<8; 
	
	GPIOE->MODER &= ~(0x03<<18);
	GPIOE->MODER |= 0x01<<18;
	GPIOE->ODR |= 0x01<<9; 
	
	GPIOE->MODER &= ~(0x03<<20);
	GPIOE->MODER |= 0x01<<20;
	GPIOE->ODR |= 0x01<<10;
	
	//BEEP
	GPIOB->MODER &= ~(0x03<<20);
	GPIOB->MODER |= 0x01<<20;
	GPIOB->ODR &= ~(0x01<<10);
#endif
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_8);
	GPIO_SetBits(GPIOE, GPIO_Pin_9);
	GPIO_SetBits(GPIOE, GPIO_Pin_10);
}

void FAN_ON(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void FAN_OFF(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void BEEP_ON(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}

void BEEP_OFF(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}

void BEEP_Circle(void)
{
	BEEP_ON();
	drv_Systick_delayms(200);
	BEEP_OFF();
	drv_Systick_delayms(200);
	
	BEEP_ON();
	drv_Systick_delayms(200);
	BEEP_OFF();
	drv_Systick_delayms(200);
	
	BEEP_ON();
	drv_Systick_delayms(200);
	BEEP_OFF();
	drv_Systick_delayms(1000);
}

void LED3_ON(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
}

void LED3_OFF(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
}
