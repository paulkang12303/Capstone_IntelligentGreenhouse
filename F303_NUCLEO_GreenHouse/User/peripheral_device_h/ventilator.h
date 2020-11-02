#ifndef	_VENTILATOR_H
#define _VENTILATOR_H

#include "stm32f30x.h"

#define VENTILATOR_SWITCH_PORT				GPIOC
#define VENTILATOR_SWITCH_PIN				GPIO_Pin_2
#define VENTILATOR_SWITCH_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)

void Ventilator_Config(void);
void Ventilator_ON(void);
void Ventilator_OFF(void);

#endif



