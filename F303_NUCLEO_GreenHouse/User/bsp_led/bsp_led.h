#ifndef _BSP_LED_H
#define	_BSP_LED_H

#include "stm32f30x.h"

#define DEBUG_LED_PORT            GPIOA 
#define DEBUG_LED_PIN             GPIO_Pin_5                                      
#define DEBUG_LED_CLK_ENABLE()    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

#define DEBUG_LED_ON()			  GPIO_SetBits(DEBUG_LED_PORT,DEBUG_LED_PIN);
#define DEBUG_LED_OFF()			  GPIO_ResetBits(DEBUG_LED_PORT,DEBUG_LED_PIN);

void DEBUG_LED_Config(void);

#endif 
