#include "I2C_sensors.h"
#include "I2C_gpio.h"
#include "bsp_uart.h"

#include "math.h"

uint8_t 	BH1750_Data[2] = {0};
float 		BH1750_Illumination = 0;

uint8_t 	HDC1080_Data[4] = {0};
float 		HDC1080_Temperature = 0;
float 		HDC1080_Humidity = 0;	
uint8_t		HDC1080_Config_Bytes[2] = {0x10,0x00};


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

void BH1750_ConvertResult( uint8_t *Data , float *Illumination)
{
	uint16_t Illu_data = 0;
	
	Illu_data = (*(Data+0) << 8 | *(Data+1));
	
	*Illumination = (float)Illu_data /1.2;
}







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

void HDC1080_ConvertResult(uint8_t *Data, float *Temperature, float *Humidity)
{
	uint16_t temp_data = 0;
	uint16_t humi_data = 0;
	
	temp_data = (*(Data+0) <<8 | *(Data+1));
	humi_data = (*(Data+2) <<8 | *(Data+3));
	
	*Temperature = (((float)temp_data)/16/16/16/16)*165 -40;
	*Humidity = ((float)humi_data)/16/16/16/16 *100;
}


