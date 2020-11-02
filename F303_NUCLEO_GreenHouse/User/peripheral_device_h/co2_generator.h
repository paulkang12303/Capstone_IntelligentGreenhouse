#ifndef _CO2_GENERATOR_H
#define _CO2_GENERATOR_H

#include "stm32f30x.h"

#define CO2_GENERATOR_SWITCH_PORT			GPIOB
#define CO2_GENERATOR_SWITCH_PIN			GPIO_Pin_15
#define CO2_GENERATOR_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

void CO2_Generator_Config(void);
void CO2_Generator_ON(void);
void CO2_Generator_OFF(void);

#endif
