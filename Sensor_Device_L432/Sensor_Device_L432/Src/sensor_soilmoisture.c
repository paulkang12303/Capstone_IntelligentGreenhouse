#include "sensor_soilmoisture.h"

uint16_t ADC_Value = 0;
uint16_t ADC_Percent = 0;

void SoilMoisture_Config(void)
{
	BSP_ADC1_Config();
}

void SoilMoisture_GetData(void)
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	ADC_Value = HAL_ADC_GetValue( &hadc1 );
	ADC_Percent = (uint16_t)(ADC_Value * 100 / 40.96);
}



