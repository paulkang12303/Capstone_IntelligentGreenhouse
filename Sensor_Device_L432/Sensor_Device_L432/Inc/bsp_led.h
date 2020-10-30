#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "main.h"

#define DEBUG_LED_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define DEBUG_LED_GPIO_PORT			GPIOB
#define DEBUG_LED_GPIO_PIN			GPIO_PIN_3

void DEBUG_LED_Config(void);
void DEBUG_LED_ON(void);
void DEBUG_LED_OFF(void);
void DEBUG_LED_TOGGLE(void);

#endif
