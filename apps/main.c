#include <stdio.h>
#include <string.h>
#include "oled.h"
#include "bmp.h"
#include "drv_systick.h"
#include "drv_key.h"
#include "drv_led.h"
#include "drv_beep.h"
#include "drv_tim.h"
#include "drv_exti.h"
#include "drv_adc.h"
#include "drv_dht11.h"
#include "drv_usart.h"
#include "FreeRTOS.h"
#include "task.h"

#define task1_stack       128
#define task2_stack       128
#define task1_uxPriority   2
#define task2_uxPriority   1
TaskHandle_t task1Handler;
TaskHandle_t task2Handler;

extern struct keys key[];
extern float light;
uint8_t ui = 0;                //三个界面的标志
_Bool ui2 = 0;
_Bool LED1_flag = 0;           //LED2的状态标志位  0--LEDOFF   1--LEDON
_Bool BEEP_flag = 0;           //BEEP的状态标志位  0--BEEPOFF  1--BEEPON

uint16_t Temperature,Humidity,a,b;
uint16_t pwmVal = 0;

u8 key1_num = '*';
u8 key2_num = '*';
u8 key3_num = '*';

u8 key1_true = '1';
u8 key2_true = '2';
u8 key3_true = '3';
u8 Cnt_Error = 0;

void drv_KEY_Proc(void)
{
	if(ui == 0)
	{
		if(key[4].flag == 1)
		{
			ui = 1;
			key[4].flag = 0;
		}
	}
	if(ui == 1)
	{
		if(key[1].flag == 1)
		{
			if(key1_num == '*')
			key1_num = 48 - 1;
			if(++key1_num >= 58)
				key1_num = 48;
			key[1].flag = 0;
		}
		if(key[2].flag == 1)
		{
			if(key2_num == '*')
			key2_num = 48 - 1;
			if(++key2_num >= 58)
				key2_num = 48;
			key[2].flag = 0;
		}
		if(key[3].flag == 1)
		{
			if(key3_num == '*')
			key3_num = 48 - 1;
			if(++key3_num >= 58)
				key3_num = 48;
			key[3].flag = 0;
		}
		if(key[4].flag == 1)
		{
			if(key1_num == key1_true && key2_num == key2_true &&key3_num == key3_true)
			{
				ui = 2;
				OLED_Refresh();
				OLED_Clear();
			}
			else
			{
				Cnt_Error++;
				if(Cnt_Error == 3)
				{
					BEEP_Circle();
					Cnt_Error = 0;
				}
			}
			key[4].flag = 0;
			key1_num = '*';
			key2_num = '*';
			key3_num = '*';
		}
	}
	if(ui == 2)
	{
		if(key[1].flag == 1)
		{
			LED1_flag =! LED1_flag;
			key[1].flag = 0;
		}
		
		if(key[2].flag == 1)
		{
			BEEP_flag =! BEEP_flag;
			key[2].flag = 0;
		}
		if(key[4].flag)
		{
			ui2 =! ui2;
			key[4].flag = 0;
		}
	}
	
	
}

void drv_LED_PWM(void)
{
	if(light > 70)
	{
		TIM_SetCompare1(TIM1,0);
		drv_Systick_delayms(1);
	}
	else if(light > 50 && light < 70)
	{
		TIM_SetCompare1(TIM1,100);
		drv_Systick_delayms(1);
	}
	else if(light > 20 && light < 50)
	{
		TIM_SetCompare1(TIM1,500);
		drv_Systick_delayms(1);
	}
	else if(light < 20)
	{
		TIM_SetCompare1(TIM1,1000);
		drv_Systick_delayms(1);
	}
//	while(pwmVal < 1000)
//	{
//		pwmVal++;
//		TIM_SetCompare1(TIM1,pwmVal);
//		drv_Systick_delayms(1);
//	}
//	while(pwmVal)
//	{
//		pwmVal--;
//		TIM_SetCompare1(TIM1,pwmVal);
//		drv_Systick_delayms(1);
//	}
//	drv_Systick_delayms(200);
}

void drv_LCD_Proc(void)
{
	drv_ADC_Value();
	DHT11_Read_Data(&Temperature,&Humidity);
	drv_LED_PWM();
	if(ui == 0)
	{
		OLED_ShowChinese(22,12,0,16,1);//华
		OLED_ShowChinese(36,12,1,16,1);//清
		OLED_ShowChinese(50,12,2,16,1);//远
		OLED_ShowChinese(64,12,3,16,1);//见
		OLED_ShowChinese(80,12,4,16,1);//长 
		OLED_ShowChinese(94,12,5,16,1);//沙
		
		OLED_ShowChinese(8,36,6,16,1);   //小
		OLED_ShowChinese(22,36,7,16,1);  //何
		OLED_ShowChinese(36,36,8,16,1);  //智
		OLED_ShowChinese(50,36,9,16,1);  //慧
		OLED_ShowChinese(64,36,10,16,1); //农
		OLED_ShowChinese(80,36,11,16,1); //业
		OLED_ShowChinese(94,36,12,16,1); //系
		OLED_ShowChinese(108,36,13,16,1);//统
		OLED_Refresh();
//		drv_Systick_delayms(1000);
		OLED_Clear();
//		OLED_ScrollDisplay(6,4,1);
	}
	else if(ui == 1)
	{
		char text[20];
		
		OLED_ShowChinese(8,0,6,16,1);   //小
		OLED_ShowChinese(22,0,7,16,1);  //何
		OLED_ShowChinese(36,0,8,16,1);  //智
		OLED_ShowChinese(50,0,9,16,1);  //慧
		OLED_ShowChinese(64,0,10,16,1); //农
		OLED_ShowChinese(80,0,11,16,1); //业
		OLED_ShowChinese(94,0,12,16,1); //系
		OLED_ShowChinese(108,0,13,16,1);//统
		
		OLED_ShowString(48,16,"K1:",16,1);
		sprintf(text,"%c",key1_num);
		OLED_ShowString(73,16,(u8 *)text,16,1);
		
		OLED_ShowString(48,32,"K2:",16,1);
		sprintf(text,"%c",key2_num);
		OLED_ShowString(73,32,(u8 *)text,16,1);
		
		OLED_ShowString(48,48,"K3:",16,1);
		sprintf(text,"%c",key3_num);
		OLED_ShowString(73,48,(u8 *)text,16,1);
		
		OLED_Refresh();
		OLED_Clear();
		
	}
	else if(ui2 == 0)
	{
		char text[20];
		OLED_ShowChinese(10,0,0,16,1); //华
		OLED_ShowChinese(28,0,1,16,1); //清
		OLED_ShowChinese(46,0,2,16,1); //远
		OLED_ShowChinese(64,0,3,16,1); //见
		OLED_ShowChinese(82,0,4,16,1); //长
		OLED_ShowChinese(100,0,5,16,1);//沙
		OLED_ShowString(121,48,"1",16,1);
		
		if(LED1_flag)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:ON",16,1);
		}
		else
		{	
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:OFF",16,1);
		}
		if(BEEP_flag)
		{
			BEEP_ON();
			OLED_ShowString(0,32,"BEEP:ON",16,1);
		}
		else
		{
			BEEP_OFF();
			OLED_ShowString(0,32,"BEEP:OFF",16,1);
		}
		OLED_ShowString(0,48,"LIGHT:",16,1);
		sprintf(text,"%.1fAD",light);
		OLED_ShowString(49,48,(u8 *)text,16,1);
		
		OLED_Refresh();
//		drv_Systick_delayms(1000);
		OLED_Clear();
		
	}
	else if(ui2 == 1)
	{
		
		OLED_ShowChinese(10,0,0,16,1); //华
		OLED_ShowChinese(28,0,1,16,1); //清
		OLED_ShowChinese(46,0,2,16,1); //远
		OLED_ShowChinese(64,0,3,16,1); //见
		OLED_ShowChinese(82,0,4,16,1); //长
		OLED_ShowChinese(100,0,5,16,1);//沙
		OLED_ShowString(121,48,"2",16,1);
		char text[20];
		OLED_ShowString(0,32,"Temp:",16,1);
		sprintf(text,"%d.%dC",Temperature>>8,Temperature&0xff);
		OLED_ShowString(41,32,(u8 *)text,16,1);
		
		OLED_ShowString(0,48,"Humi:",16,1);
		sprintf(text,"%d.%dAH",Humidity>>8,Humidity&0xff);
		OLED_ShowString(41,48,(u8 *)text,16,1);
		
		OLED_Refresh();
//		drv_Systick_delayms(1000);
		OLED_Clear();
	}
	
}

void drv_FAN_Proc(void)
{
	if(Temperature>>8 > 35)
	{
		FAN_ON();
		BEEP_Circle();
	}
	else
	{
		FAN_OFF();
		BEEP_OFF();
	}
}

void drv_ESP8266_SendByte(void)
{
	drv_Systick_delayms(1000);
	drv_USART_SendStr(USART2,"AT+RST\r\n",strlen("AT+RST\r\n"));
	drv_Systick_delayms(500);
	drv_USART_SendStr(USART2,"AT\r\n",strlen("AT\r\n"));
	drv_Systick_delayms(500);
	drv_USART_SendStr(USART2,"AT+CWMODE=1\r\n",strlen("AT+CWMODE=1\r\n"));
	drv_Systick_delayms(500);
	drv_USART_SendStr(USART2,"AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"));
	drv_Systick_delayms(500);
	drv_USART_SendStr(USART2,"AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n",strlen("AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n"));
	drv_Systick_delayms(5000);
	drv_USART_SendStr(USART2,"AT+CIPSTART=\"TCP\",\"192.168.90.136\",9090\r\n",strlen("AT+CIPSTART=\"TCP\",\"192.168.90.136\",9090\r\n"));
	drv_Systick_delayms(2000);
	drv_USART_SendStr(USART2,"AT+CIPSEND=6\r\n",strlen("AT+CIPSEND=6\r\n"));
	drv_Systick_delayms(500);
	drv_USART_SendStr(USART2,"HEJIAN\r\n",strlen("HEJIAN\r\n"));
	drv_Systick_delayms(500);
}



int main()
{
	drv_Systick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	drv_KEY_Init();
 	OLED_Init();		
	drv_LED_Init();
	drv_BEEP_Init();
	drv_TIM1_Init();
	drv_TIM6_Init();
	drv_ADC1_Init();
	drv_DHT11_Init();
	drv_USART1_Init();
	drv_USART2_Init();
	
//	drv_ESP8266_SendByte();
	
//	drv_EXTI4_Init();
	while(1) 
	{
		drv_LCD_Proc();
		drv_KEY_Proc();
		drv_FAN_Proc();
	}
	
}
