#include "airconditioner.h"

void Heater_Config(void)
{		
	HEATER_SWITCH_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = HEATER_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(HEATER_SWITCH_PORT, &GPIO_InitStruct);
}

void Heater_ON(void)
{
	GPIO_SetBits(HEATER_SWITCH_PORT, HEATER_SWITCH_PIN);
}

void Heater_OFF(void)
{
	GPIO_ResetBits(HEATER_SWITCH_PORT, HEATER_SWITCH_PIN);
}

void Heater_SendCommand(uint16_t AimTemperature)
{
	printf("The Heater will raise temperature to %d Celsius",AimTemperature);
}


void Cooler_Config(void)
{		
	COOLER_SWITCH_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = COOLER_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(COOLER_SWITCH_PORT, &GPIO_InitStruct);
}

void Cooler_ON(void)
{
	GPIO_SetBits(COOLER_SWITCH_PORT, COOLER_SWITCH_PIN);
}

void Cooler_OFF(void)
{
	GPIO_ResetBits(COOLER_SWITCH_PORT, COOLER_SWITCH_PIN);
}

void Cooler_SendCommand(uint16_t AimTemperature)
{
	printf("The Cooler will decrease temperature to %d Celsius",AimTemperature);
}





