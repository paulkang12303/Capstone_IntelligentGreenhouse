#include "sensor_hdc1080.h"

uint8_t 	HDC1080_Data[4] = {0};
float 		HDC1080_Temperature = 0;
float 		HDC1080_Humidity = 0;	
uint16_t	HDC1080_Temperature_TX = 0;
uint16_t	HDC1080_Humidity_TX = 0;
uint8_t		HDC1080_Config_Bytes[2] = {0x10,0x00};

void HDC1080_Config(void)
{
	I2C_BUS_2_Config();
	
	I2C_BUS_2_WriteDevice(0x80,0x02,HDC1080_Config_Bytes,2);
}

void HDC1080_Start(void)
{
	I2C_BUS_2_ReadDevice_Pre(0x80,0x00);
}

void HDC1080_WaitingData_ms(void)
{
	HAL_Delay(20);
}

void HDC1080_ReadData(uint8_t *Data)
{
	I2C_BUS_2_ReadDevice_Post(0x80,Data,4);
}

void HDC1080_GetResult(uint8_t *Data, float *Temperature, float *Humidity)
{
	uint16_t temp_data = 0;
	uint16_t humi_data = 0;
	
	temp_data = (*(Data+0) <<8 | *(Data+1));
	humi_data = (*(Data+2) <<8 | *(Data+3));
	
	*Temperature = (((float)temp_data)/16/16/16/16)*165 -40;
	*Humidity = ((float)humi_data)/16/16/16/16 *100;
}

void HDC1080_ConvertResultToInteger(float *Temperature, float *Humidity, uint16_t *Result_T, uint16_t *Result_H)
{
	*Result_T = (uint16_t)(*Temperature * 100);
	*Result_H = (uint16_t)(*Humidity * 100);
}

