#ifndef _ALARM_H
#define _ALARM_H

#include "stm32f30x.h"

#define ALARM_LIGHT_PORT            	GPIOA 
#define ALARM_LIGHT_PIN             	GPIO_Pin_5                                      
#define ALARM_LIGHT_CLK_ENABLE()    	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

void ALARM_LIGHT_ON(void);
void ALARM_LIGHT_OFF(void);
void ALARM_Config(void);

#endif
