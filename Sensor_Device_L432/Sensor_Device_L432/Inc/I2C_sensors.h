#ifndef _I2C_SENSORS_H
#define _I2C_SENSORS_H

#include "stm32l4xx_hal.h"

void SHT30_Config(void);
void SHT30_Start(void);
void SHT30_WaitingData_ms(void);
void SHT30_ReadData( uint8_t *Data );
void SHT30_ConvertResult( uint8_t *Data, float *Temperature, float *Humidity);

void BH1750_Config(void);
void BH1750_Start(void);
void BH1750_WaitingData_ms(void);
void BH1750_ReadData( uint8_t *Data );
void BH1750_ConvertResult( uint8_t *Data , float *Illumination);

void HDC1080_Config(void);
void HDC1080_Start(void);
void HDC1080_WaitingData_ms(void);
void HDC1080_ReadData(uint8_t *Data);
void HDC1080_ConvertResult(uint8_t *Data, float *Temperature, float *Humidity);


#endif
