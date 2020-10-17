#ifndef _ENVIRONMENT_DEVICE_H
#define _ENVIRONMENT_DEVICE_H

#include "stm32f30x.h"

#define ALARM_LED_PORT            GPIOA 
#define ALARM_LED_PIN             GPIO_Pin_5                                      
#define ALARM_LED_CLK_ENABLE()    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

void ALARM_LIGHT_ON(void);
void ALARM_LIGHT_OFF(void);
void ALARM_Config(void);
#endif
