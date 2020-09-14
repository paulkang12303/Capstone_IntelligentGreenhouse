
#include "bsp_led.h"
  

void LED_TEST_Config(void)
{		
	RCC_AHBPeriphClockCmd(LED_TEST_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = LED_TEST_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(LED_TEST_GPIO, &GPIO_InitStruct);
}

void LED_TEST_ON(void)
{
	GPIO_WriteBit(LED_TEST_GPIO, LED_TEST_PIN, Bit_SET);
}

void LED_TEST_OFF(void)
{
	GPIO_WriteBit(LED_TEST_GPIO, LED_TEST_PIN, Bit_RESET);
}