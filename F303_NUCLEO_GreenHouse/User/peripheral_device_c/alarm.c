#include "alarm.h"

void ALARM_Config(void)
{		
	ALARM_LIGHT_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = ALARM_LIGHT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(ALARM_LIGHT_PORT, &GPIO_InitStruct);
}

void ALARM_LIGHT_ON(void)
{
	GPIO_SetBits(ALARM_LIGHT_PORT, ALARM_LIGHT_PIN);
}

void ALARM_LIGHT_OFF(void)
{
	GPIO_ResetBits(ALARM_LIGHT_PORT, ALARM_LIGHT_PIN);
}


