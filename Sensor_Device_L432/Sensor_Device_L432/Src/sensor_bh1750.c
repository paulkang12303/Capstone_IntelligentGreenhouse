#include "sensor_bh1750.h"

uint8_t 	BH1750_Data[2] = {0};
float 		BH1750_Illumination = 0;
uint16_t	BH1750_Illumination_TX = 0;

void BH1750_Config(void)
{
	I2C_BUS_1_Config();
}

void BH1750_Start(void)
{
	I2C_BUS_1_ReadDevice_Pre(0x46,0x01);	// power on BH1750
	I2C_BUS_1_ReadDevice_Pre(0x46,0x20);	// OneTime H-Resolution Start
}

void BH1750_WaitingData_ms(void)
{
	HAL_Delay(180);
}

void BH1750_ReadData( uint8_t *Data )
{
	I2C_BUS_1_ReadDevice_Post(0x46,Data,2);
}

void BH1750_GetResult( uint8_t *Data , float *Illumination)
{
	uint16_t Illu_data = 0;
	
	Illu_data = (*(Data+0) << 8 | *(Data+1));
	
	*Illumination = (float)Illu_data /1.2;
}

void BH1750_ConvertResultToInteger(float *Illumination, uint16_t *Result)
{
	*Result = (uint16_t)(*Illumination * 10);
}

