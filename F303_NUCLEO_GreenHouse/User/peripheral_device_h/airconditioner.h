#ifndef _AIRCONDITIONER_H
#define _AIRCONDITIONER_H

#include "stm32f30x.h"
#include "stdio.h"

#define HEATER_SWITCH_PORT			GPIOC
#define HEATER_SWITCH_PIN			GPIO_Pin_0
#define HEATER_SWITCH_CLK_ENABLE()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)

#define COOLER_SWITCH_PORT			GPIOC
#define COOLER_SWITCH_PIN			GPIO_Pin_1
#define COOLER_SWITCH_CLK_ENABLE()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)

void Heater_Config(void);
void Heater_ON(void);
void Heater_OFF(void);
void Heater_SendCommand(uint16_t AimTemperature);

void Cooler_Config(void);
void Cooler_ON(void);
void Cooler_OFF(void);
void Cooler_SendCommand(uint16_t AimTemperature);
#endif
