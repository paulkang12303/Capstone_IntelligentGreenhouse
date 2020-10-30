
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l4xx_hal.h"

void Error_Handler(void);

#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA


#define MAIN_TASK__NONE						0
#define MAIN_TASK__DEAL_WITH_MAIN_TIMER		1
#define MAIN_TASK__MANAGE_DATA				2

void main_task__idle(void);
void main_task__deal_with_main_timer(void);
void main_task__manage_data(void);













#endif
