#ifndef _SENSOR_SOILMOISTURE_H
#define _SENSOR_SOILMOISTURE_H

#include "adc.h"

extern uint16_t ADC_Value;
extern uint16_t ADC_Percent;

void SoilMoisture_Config(void);
void SoilMoisture_GetData(void);

#endif
