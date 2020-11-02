#include "ventilator.h"

void Ventilator_Config(void)
{
	VENTILATOR_SWITCH_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = VENTILATOR_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(VENTILATOR_SWITCH_PORT, &GPIO_InitStruct);
}

void Ventilator_ON(void)
{
	GPIO_SetBits(VENTILATOR_SWITCH_PORT, VENTILATOR_SWITCH_PIN);
}

void Ventilator_OFF(void)
{
	GPIO_ResetBits(VENTILATOR_SWITCH_PORT, VENTILATOR_SWITCH_PIN);
}

