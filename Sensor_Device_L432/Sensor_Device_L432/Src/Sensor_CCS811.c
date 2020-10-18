#include "Sensor_CCS811.h"
#include "I2C_gpio.h"
#include <stdio.h>

uint8_t BUF[12];
uint8_t Information[10];
uint8_t MeasureMode, Status, Error_ID;
uint8_t FlagGetId = 1;
uint8_t n         = 4; 
uint8_t temp      = 0x5a;
CCS811_Measurement CCS811;

void CCS811_CS_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	CCS811_CS_CLK_ENABLE();

	GPIO_InitStruct.Pin = CCS811_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(CCS811_CS_PORT, &GPIO_InitStruct);
	
	CCS811_CS_OFF();
}

void CCS811_Config(void)
{
	I2C_BUS_2_Config();
	CCS811_CS_GPIO_Config();
	HAL_Delay(3000);
	uint8_t ID_Count = 0;
	CCS811_CS_ON();
	CCS811_STEP_DELAY();
	while(FlagGetId)
	{
		I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x20);
		I2C_BUS_2_ReadDevice_Post(CCS811_Add,Information,1);
		if(Information[0] == 0x81)
		{
			if(++ID_Count == n)
			{
				FlagGetId = 0;
			}
			else
			{
				printf("id=%d,correct %d!\r\n", Information[0], ID_Count);
			}
		}
		else
		{
			printf("id=%d,incorrect,continuing...\r\n", Information[0]);
		}
		CCS811_STEP_DELAY();
	}
	printf("id correct,initing...\r\n");
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x23);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Information[1],2);
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x24);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Information[3],2);	
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x00);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Status,1);
	CCS811_STEP_DELAY();
	
	if(Status & 0x10)
	{

		while(!(Status & 0x80)) // if firmware not in application mode but boot mode.
		{
			I2C_BUS_2_WriteDevide_SingleByte(CCS811_Add, 0xF3, 0xF0);	      // Application Verify
			printf("trying to transition the CCS811 state from boot to application mode...\r\n");
			I2C_BUS_2_WriteDevice(CCS811_Add, 0xF4, &temp, 0);	//Used to transition the CCS811 state from boot to application mode, a write with no data is required.
			CCS811_STEP_DELAY();
			I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x00);
			I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Status,1);
			CCS811_STEP_DELAY();
		}
		printf("CCS811 is already in application mode!\r\n");
	}	
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x01);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&MeasureMode,1);
	CCS811_STEP_DELAY();
	MeasureMode &= 0x70;
	
	while(MeasureMode != DRIVE_MODE_1SEC)
	{
		I2C_BUS_2_WriteDevide_SingleByte(CCS811_Add, 0x01, DRIVE_MODE_1SEC); 
		CCS811_STEP_DELAY();
		I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x01);
		I2C_BUS_2_ReadDevice_Post(CCS811_Add,&MeasureMode,1);
		MeasureMode &= 0x70;
		printf("trying to enter measure mode...\r\n");
		CCS811_STEP_DELAY();
	}
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x00);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Status,1);	
	CCS811_STEP_DELAY();	
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x01);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&MeasureMode,1);	
	CCS811_STEP_DELAY();	
	
	CCS811_CS_OFF();
	CCS811_STEP_DELAY();
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0xE0);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Error_ID,1);	
	printf("status=%d error_id=%d measureMode=%d \r\n", Status, Error_ID, MeasureMode);
	
}


void CCS811GetData(void)
{
	CCS811_CS_ON(); 	// nWAKE pin is asserted at least 50¦Ìs before the transaction and kept asserted throughout,nWAKE pin is active low
	HAL_Delay(10);
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x02);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,BUF,8);	
	HAL_Delay(10);
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0x20);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,Information,1);	
	HAL_Delay(10);
	I2C_BUS_2_ReadDevice_Pre(CCS811_Add,0xE0);
	I2C_BUS_2_ReadDevice_Post(CCS811_Add,&Error_ID, 1);		
	CCS811_CS_OFF();
	CCS811.eco2 = (uint16_t)BUF[0] * 256 + BUF[1];
	CCS811.tvoc = (uint16_t)BUF[2] * 256 + BUF[3];
	CCS811.device_id  = Information[0];
	CCS811.error_id   = Error_ID;
	Error_ID       = 0;
	Information[0] = 0;
}

void CCS811ClearData(void)
{
	CCS811.device_id = 0;
	CCS811.eco2      = 0;
	CCS811.status    = 0;
	CCS811.tvoc      = 0;
	CCS811.error_id  = 0;
}





