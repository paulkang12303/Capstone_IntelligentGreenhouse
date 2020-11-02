#ifndef _WATER_SPRAYER_H
#define _WATER_SPRAYER_H

#include "stm32f30x.h"

#define WATER_SPRAYER_SWITCH_PORT			GPIOC
#define WATER_SPRAYER_SWITCH_PIN			GPIO_Pin_3
#define WATER_SPRAYER_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)

void Water_Sprayer_Config(void);
void Water_Sprayer_ON(void);
void Water_Sprayer_OFF(void);

#endif
