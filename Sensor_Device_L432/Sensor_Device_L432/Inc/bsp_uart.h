#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "stdio.h"
#include "stm32l4xx_hal.h"

#define		USART_DEBUG_SN					USART2
#define 	DebugUART_CLK_ENABLE()			__HAL_RCC_USART2_CLK_ENABLE()
#define		DebugUART_TX_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define		DebugUART_RX_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define		DebugUART_TX_PORT				GPIOA
#define		DebugUART_RX_PORT				GPIOA
#define		DebugUART_TX_PIN				GPIO_PIN_2
#define		DebugUART_RX_PIN				GPIO_PIN_15

extern UART_HandleTypeDef DebugUART;

void DEBUG_UART_Config(void);
void sendByte_via(UART_HandleTypeDef *huart,uint8_t *data_TX);
#endif
