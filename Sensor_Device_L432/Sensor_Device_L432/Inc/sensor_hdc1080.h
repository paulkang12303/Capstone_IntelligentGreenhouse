#ifndef SENSOR_HDC1080_H
#define SENSOR_HDC1080_H

#include "stm32l4xx_hal.h"

#include "math.h"
#include "bsp_uart.h"
#include "i2c_gpio.h"

extern 			uint8_t HDC1080_Data[4];
extern float 	HDC1080_Temperature;
extern float 	HDC1080_Humidity;
extern uint16_t	HDC1080_Temperature_TX;
extern uint16_t	HDC1080_Humidity_TX;
	
void HDC1080_Config(void);
void HDC1080_Start(void);
void HDC1080_WaitingData_ms(void);
void HDC1080_ReadData(uint8_t *Data);
void HDC1080_GetResult(uint8_t *Data, float *Temperature, float *Humidity);
void HDC1080_ConvertResultToInteger(float *Temperature, float *Humidity, uint16_t *Result_T, uint16_t *Result_H);

#endif
