#ifndef _SENSOR_BH1750_H
#define _SENSOR_BH1750_H

#include "stm32l4xx_hal.h"

#include "math.h"
#include "bsp_uart.h"
#include "i2c_gpio.h"

extern 			uint8_t BH1750_Data[2];
extern float 	BH1750_Illumination;
extern uint16_t	BH1750_Illumination_TX;

void BH1750_Config(void);
void BH1750_Start(void);
void BH1750_WaitingData_ms(void);
void BH1750_ReadData( uint8_t *Data );
void BH1750_GetResult( uint8_t *Data , float *Illumination);
void BH1750_ConvertResultToInteger(float *Illumination, uint16_t *Result);

#endif
