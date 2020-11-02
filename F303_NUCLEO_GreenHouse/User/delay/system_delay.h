#ifndef _SYSTEM_DELAY_H
#define _SYSTEM_DELAY_H

#include "FreeRTOS.h"
#include "task.h"

#define SystemDelay_ms(n)		vTaskDelay(n)

#endif
