#include "uln2003_28BYJ_48.h"



void ULN2003_Config(void)
{
	ULN2003_ORANGE_PIN_CLK_ENABLE();
	ULN2003_YELLOW_PIN_CLK_ENABLE();
	ULN2003_PINK_PIN_CLK_ENABLE();
	ULN2003_BLUE_PIN_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	
	GPIO_InitStruct.GPIO_Pin = ORANGE_PIN;
	GPIO_Init(ORANGE_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = YELLOW_PIN;
	GPIO_Init(YELLOW_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = PINK_PIN;
	GPIO_Init(PINK_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = BLUE_PIN;
	GPIO_Init(BLUE_PORT, &GPIO_InitStruct);
	
	ULN2003_OFF();
}

void ULN2003_OFF(void)
{
	Orange_L();
	Yellow_L();	
	Pink_L();
	Blue_L(); 
}

void ULN2003_ForwardRun(uint16_t period)
{
	Orange_H();
	ULN2003_Delay_ms(period);
	Blue_L();
	ULN2003_Delay_ms(period);
	Yellow_H();
	ULN2003_Delay_ms(period);
	Orange_L();
	ULN2003_Delay_ms(period);
	Pink_H();
	ULN2003_Delay_ms(period);
	Yellow_L();
	ULN2003_Delay_ms(period);
	Blue_H();
	ULN2003_Delay_ms(period);
	Pink_L();
	ULN2003_Delay_ms(period);
}

void ULN2003_BackwardRun(uint16_t period)
{
	Blue_H();
	ULN2003_Delay_ms(period);
	Orange_L();
	ULN2003_Delay_ms(period);
	Pink_H();
	ULN2003_Delay_ms(period);
	Blue_L();
	ULN2003_Delay_ms(period);
	Yellow_H();
	ULN2003_Delay_ms(period);
	Pink_L();
	ULN2003_Delay_ms(period);
	Orange_H();
	ULN2003_Delay_ms(period);
	Yellow_L();
	ULN2003_Delay_ms(period);
}

void ULN2003_Forword360(uint16_t period)
{
	for(uint16_t j=0;j<512;j++) 
	{
		ULN2003_ForwardRun(period);
	}
}

void ULN2003_Backword360(uint16_t period)
{
	for(uint16_t j=0;j<512;j++) 
	{
		ULN2003_BackwardRun(period);
	}
}


uint8_t ULN2003_ToTerminal(void)
{
	ULN2003_Forword360(1);
	return 1;
}

uint8_t ULN2003_ToInitial(void)
{
	ULN2003_Backword360(1);
	return 1;
}






