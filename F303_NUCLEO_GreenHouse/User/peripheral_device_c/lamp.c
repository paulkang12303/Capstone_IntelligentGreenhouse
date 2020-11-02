#include "lamp.h"

void Lamp_Config(void)
{		
	LAMP_SWITCH_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = LAMP_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(LAMP_SWITCH_PORT, &GPIO_InitStruct);
}

void Lamp_ON(void)
{
	GPIO_SetBits(LAMP_SWITCH_PORT, LAMP_SWITCH_PIN);
}

void Lamp_OFF(void)
{
	GPIO_ResetBits(LAMP_SWITCH_PORT, LAMP_SWITCH_PIN);
}
