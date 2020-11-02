#ifndef _LAMP_H
#define _LAMP_H

#include "stm32f30x.h"

#define LAMP_SWITCH_PORT			GPIOB
#define LAMP_SWITCH_PIN				GPIO_Pin_10
#define LAMP_SWITCH_CLK_ENABLE()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

void Lamp_Config(void);
void Lamp_ON(void);
void Lamp_OFF(void);

#endif
