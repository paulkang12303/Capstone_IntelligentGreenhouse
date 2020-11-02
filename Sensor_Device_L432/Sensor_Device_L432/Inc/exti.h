#ifndef _EXTI_H
#define _EXTI_H

#include "stm32l4xx_hal.h"

#define RUN_TRIGGER_PORT	GPIOA
#define RUN_TRIGGER_PIN		GPIO_PIN_11

void EXTI_PA11_Config(void);

#endif
