
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l4xx_hal.h"

#include "stdio.h"

#include "timer.h"
#include "exti.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "sensor_CCS811.h"
#include "sensor_bh1750.h"
#include "sensor_hdc1080.h"
#include "sensor_soilmoisture.h"

void Error_Handler(void);

#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA


#define MAIN_TASK__NONE						0
#define MAIN_TASK__DEAL_WITH_TRIGGER		1
#define MAIN_TASK__MANAGE_DATA				2

void main_task__idle(void);
void main_task__deal_with_trigger(void);
void main_task__manage_data(void);













#endif
