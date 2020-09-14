#ifndef _BSP_LED_H
#define	_BSP_LED_H

#include "stm32f30x.h"

#define	LED_TEST_CLK			RCC_AHBPeriph_GPIOA
#define LED_TEST_GPIO			GPIOA
#define LED_TEST_PIN			GPIO_Pin_5

void LED_TEST_Config(void);
void LED_TEST_ON(void);
void LED_TEST_OFF(void);

#endif 
