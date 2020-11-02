#include "co2_generator.h"

void CO2_Generator_Config(void)
{
	CO2_GENERATOR_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = CO2_GENERATOR_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(CO2_GENERATOR_SWITCH_PORT, &GPIO_InitStruct);
}

void CO2_Generator_ON(void)
{
	GPIO_SetBits(CO2_GENERATOR_SWITCH_PORT, CO2_GENERATOR_SWITCH_PIN);
}

void CO2_Generator_OFF(void)
{
	GPIO_ResetBits(CO2_GENERATOR_SWITCH_PORT, CO2_GENERATOR_SWITCH_PIN);
}

