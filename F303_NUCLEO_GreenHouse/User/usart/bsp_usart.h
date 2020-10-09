#ifndef _USART_H
#define	_USART_H

#include <stdio.h>
#include "stm32f30x.h"

#define		STM32F303_NUCLEO		1

#ifdef		STM32F303_NUCLEO
	#define DEBUG_USART_IRQn				USART2_IRQn
	#define DEBUG_USART_CLK_ENABLE()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE)
	#define	DEBUG_USART_GPIO_CLK_ENABLE()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)
	#define	DEBUG_USART_TX_PIN				GPIO_Pin_2
	#define	DEBUG_USART_RX_PIN				GPIO_Pin_3
	#define	DEBUG_USART_TX_PORT				GPIOA
	#define	DEBUG_USART_RX_PORT				GPIOA
	#define	DEBUG_USART_TX_PIN_SOURCE		GPIO_PinSource2
	#define	DEBUG_USART_RX_PIN_SOURCE		GPIO_PinSource3
	#define DEBUG_USART_ID					USART2
#endif

void DEBUG_USART_Config(void);

#endif
