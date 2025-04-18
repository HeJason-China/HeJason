#include "drv_systick.h"

static u8 fs_us = 0;
static u16 fs_ms = 0;

//Systick计时器的初始化
void drv_Systick_Init(u8 sysclk)
{
	SysTick->CTRL &= ~(0x05);
	fs_us = sysclk / 8;
	fs_ms = fs_us * 1000;
	
}

//毫秒计时器
void drv_Systick_delayxms(u32 nms)
{
	u32 temp = 0;
	SysTick->LOAD = (u32)fs_ms*nms;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}
	while(!(temp&(0x01<<16)));
	SysTick->CTRL &= ~0x01;
}

void drv_Systick_delayms(u16 nms)
{
	u8 repeat = nms/540;
	u16 remain = nms%540;
	while(repeat)
	{
		drv_Systick_delayxms(540);
		repeat--;
	}
	if(remain)
		drv_Systick_delayxms(remain);
}

//微秒计时器
void drv_Systick_delayus(u32 nus)
{
	u32 temp = 0;
	SysTick->LOAD = fs_us*nus;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= 0x01;
	do
	{
		temp = SysTick->CTRL;
	}
	while(!(temp&(0x01<<16)));
	SysTick->CTRL &= ~0x01;
	SysTick->VAL = 0x00;
}
