#ifndef _BSP_KEY_H
#define	_BSP_KEY_H

#include "stm32f30x.h"

#define 	KEY1_GPIO_PORT 				GPIOA
#define		KEY1_GPIO_PIN				GPIO_Pin_0
#define 	KEY1_GPIO_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

#define		KEY2_GPIO_PORT 				GPIOA
#define		KEY2_GPIO_PIN				GPIO_Pin_1
#define 	KEY2_GPIO_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

#define		KEY_DOWN		0
#define		KEY_UP			1

void DEBUG_KEY_Config(void);
uint8_t Debug_Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif 
