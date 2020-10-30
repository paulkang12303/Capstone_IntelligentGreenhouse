#ifndef	_TIMER_H
#define _TIMER_H

#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef SysMainTimer;

void SystemMainTimer_Config(void);

#endif
