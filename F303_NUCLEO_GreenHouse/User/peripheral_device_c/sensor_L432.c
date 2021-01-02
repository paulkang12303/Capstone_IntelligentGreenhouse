#include "sensor_L432.h"

void sensor_l432_Config(void)
{		
	SENSORL432_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = SENSORL432_TRIGGER_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(SENSORL432_TRIGGER_PORT, &GPIO_InitStruct);
}

void Sensorl432_ON(void)
{
	GPIO_SetBits(SENSORL432_TRIGGER_PORT, SENSORL432_TRIGGER_PIN);
}

void Sensorl432_OFF(void)
{
	GPIO_ResetBits(SENSORL432_TRIGGER_PORT, SENSORL432_TRIGGER_PIN);
}


