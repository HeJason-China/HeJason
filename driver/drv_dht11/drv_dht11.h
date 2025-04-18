#ifndef __DRV_DHT11_H
#define __DRV_DHT11_H

#include "stm32f4xx.h"

#define    DHT11_DQ_OUT_HIGH    GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define    DHT11_DQ_OUT_LOW 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)

#define    DHT11_DQ_IN	        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)

//IO方向设置
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

   	
uint8_t drv_DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11  

#endif
