#ifndef _ENVIRONMENT_DEVICE_H
#define _ENVIRONMENT_DEVICE_H

#include "stm32f30x.h"


#define LAMP_SWITCH_PORT			GPIOB
#define LAMP_SWITCH_PIN				GPIO_Pin_10
#define LAMP_SWITCH_CLK_ENABLE()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

#define ALARM_LED_PORT            	GPIOA 
#define ALARM_LED_PIN             	GPIO_Pin_5                                      
#define ALARM_LED_CLK_ENABLE()    	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

void LampSwitch_Config(void);
void LampSwitch_ON(void);
void LampSwitch_OFF(void);

void ALARM_LIGHT_ON(void);
void ALARM_LIGHT_OFF(void);
void ALARM_Config(void);
#endif
