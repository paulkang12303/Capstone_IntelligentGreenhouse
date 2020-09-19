#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "main.h"

#define LED_TEST_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_TEST_GPIO_PORT			GPIOB
#define LED_TEST_GPIO_PIN			GPIO_PIN_3

void LED_TEST_Config(void);
void LED_TEST_ON(void);
void LED_TEST_OFF(void);

#endif
