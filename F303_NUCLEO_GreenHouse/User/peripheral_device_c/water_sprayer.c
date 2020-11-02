#include "water_sprayer.h"

void Water_Sprayer_Config(void)
{
	WATER_SPRAYER_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = WATER_SPRAYER_SWITCH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(WATER_SPRAYER_SWITCH_PORT, &GPIO_InitStruct);
}

void Water_Sprayer_ON(void)
{
	GPIO_SetBits(WATER_SPRAYER_SWITCH_PORT, WATER_SPRAYER_SWITCH_PIN);
}

void Water_Sprayer_OFF(void)
{
	GPIO_ResetBits(WATER_SPRAYER_SWITCH_PORT, WATER_SPRAYER_SWITCH_PIN);
}


