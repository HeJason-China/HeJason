#include "drv_usart.h"
#include "drv_led.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

void drv_USART1_Init(void)
{
	//定义GPIO、USART的结构体，进行参数的配置
	GPIO_InitTypeDef GPIO_InitStruct;//GPIO结构体的声明
	USART_InitTypeDef USART_InitStruct;//串口声明
	NVIC_InitTypeDef NVIC_InitStruct;//中断控制器的声明
	//开启GPIOA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//PA9--TX、PA10--RX的初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//配置PA9、PA10具体的复用功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//串口USART1的初始化
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	//使能USART1
	USART_Cmd(USART1,ENABLE);
	
}

void drv_USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//开启GPIOD的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//开启USART2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//PD5--TX、PD6--RX的初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	//配置PD5、PD6具体的复用功能
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	//开启接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//开启空闲接收中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ClearFlag(USART2,USART_FLAG_TC | USART_FLAG_TXE);
	//使能USART2
	USART_Cmd(USART2,ENABLE);
}

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};
/* FILE is typedef'd in stdio.h. */
FILE __stdout;
int fputc(int ch, FILE *f)
{
  /* Your implementation of fputc(). */
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	USART_ClearFlag(USART1,USART_FLAG_TC);
  return ch;
}

//uint8_t rx_buffer[BUFFER_SIZE] = {0};
//uint8_t rx_index = 0;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_SendData(USART1,USART_ReceiveData(USART1));
//		if(rx_index < BUFFER_SIZE-1)
//		{
//			rx_buffer[rx_index] = USART_ReceiveData(USART1);
//			rx_index++;
//		}
//		rx_index = 0;
//		memset(rx_buffer,0,sizeof(rx_buffer));
	}
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		USART_ReceiveData(USART1);
	}
}

uint8_t USART2_BUF[256];
uint8_t USART2_Index = 0;
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART_SendData(USART1,USART_ReceiveData(USART2));
		USART2_BUF[USART2_Index] = USART_ReceiveData(USART2);
		
		if(USART2_BUF[0] == '+')
		{
			USART2_Index++;
		}
		else
		{
			memset(USART2_BUF,0,sizeof(USART2_BUF));
			USART2_Index = 0;
		}
		
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE))
	{
		USART_ReceiveData(USART2);
		if(strcmp((const char *)USART2_BUF,"+IPD,8:LED3ON\r\n")==0)
		{
			LED3_ON();
		}
		else if(strcmp((const char *)USART2_BUF,"+IPD,9:LED3OFF\r\n")==0)
		{
			LED3_OFF();
		}
		else if(strcmp((const char *)USART2_BUF,"+IPD,8:BEEPON\r\n")==0)
		{
			BEEP_ON();
		}
		else if(strcmp((const char *)USART2_BUF,"+IPD,9:BEEPOFF\r\n")==0)
		{
			BEEP_OFF();
		}
		memset(USART2_BUF,0,sizeof(USART2_BUF));
	}
	
}

void drv_USART_SendStr(USART_TypeDef* USARTx,char *str,int size)
{
	uint8_t i=0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	for(i=0;i<size;i++)
	{
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TC);
		str++;
	}
	
}

