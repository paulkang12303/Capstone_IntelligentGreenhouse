#ifndef _SENSOR_L432_H
#define _SENSOR_L432_H

#include "stm32f30x.h"

#define SENSORL432_TRIGGER_PORT		GPIOC
#define SENSORL432_TRIGGER_PIN		GPIO_Pin_6
#define SENSORL432_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)

void sensor_l432_Config(void);
void Sensorl432_ON(void);
void Sensorl432_OFF(void);


#endif
