
#include "bsp_uart.h"

UART_HandleTypeDef DebugUART;

void DEBUG_UART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	DebugUART_TX_GPIO_CLK_ENABLE();
	DebugUART_RX_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = DebugUART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(DebugUART_TX_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DebugUART_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
    HAL_GPIO_Init(DebugUART_RX_PORT, &GPIO_InitStruct);
	

	DebugUART_CLK_ENABLE();
	
	DebugUART.Instance = USART_DEBUG_SN;
	DebugUART.Init.BaudRate = 115200;
	DebugUART.Init.WordLength = UART_WORDLENGTH_8B;
	DebugUART.Init.StopBits = UART_STOPBITS_1;
	DebugUART.Init.Parity = UART_PARITY_NONE;
	DebugUART.Init.Mode = UART_MODE_TX_RX;
	DebugUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	DebugUART.Init.OverSampling = UART_OVERSAMPLING_16;
	DebugUART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	DebugUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&DebugUART);
	
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&DebugUART, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

void sendByte_via(UART_HandleTypeDef *huart,uint8_t *data_TX)
{
	HAL_UART_Transmit(huart, data_TX, 1, 0xFFFF);
}

void sendString_via(UART_HandleTypeDef *huart,uint8_t *data_TX)
{
	while(*data_TX)
	{
		HAL_UART_Transmit(huart,(uint8_t *)data_TX++,1,0xFFFF);
	}
}


