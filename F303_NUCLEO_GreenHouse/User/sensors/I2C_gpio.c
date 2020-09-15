#include "I2C_gpio.h"

static void I2C_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 60; i++);
}

void SENSOR_1_I2C_Start(void)
{
	SENSOR_1_I2C_SDA_1();I2C_Delay();
	SENSOR_1_I2C_SCL_1();I2C_Delay();
	SENSOR_1_I2C_SDA_0();I2C_Delay();
	SENSOR_1_I2C_SCL_0();I2C_Delay();
}

void SENSOR_1_I2C_Stop(void)
{
	SENSOR_1_I2C_SDA_0();I2C_Delay();
	SENSOR_1_I2C_SCL_1();I2C_Delay();
	SENSOR_1_I2C_SDA_1();I2C_Delay();
}

void SENSOR_1_I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			SENSOR_1_I2C_SDA_1();I2C_Delay();
		}
		else
		{
			SENSOR_1_I2C_SDA_0();I2C_Delay();
		}
		SENSOR_1_I2C_SCL_1();I2C_Delay();
		SENSOR_1_I2C_SCL_0();
		if (i == 7)
		{
			 SENSOR_1_I2C_SDA_1();
		}
		_ucByte <<= 1;
		I2C_Delay();
	}
}

uint8_t SENSOR_1_I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SENSOR_1_I2C_SCL_1();I2C_Delay();
		if (SENSOR_1_I2C_SDA_READ())
		{
			value++;
		}
		SENSOR_1_I2C_SCL_0();I2C_Delay();
	}
	return value;
}

uint8_t SENSOR_1_I2C_WaitAck(void)
{
	uint8_t feedback;

	SENSOR_1_I2C_SDA_1();I2C_Delay();
	SENSOR_1_I2C_SCL_1();I2C_Delay();

	if (SENSOR_1_I2C_SDA_READ())
	{
		feedback = 1;
	}
	else
	{
		feedback = 0;
	}

	SENSOR_1_I2C_SCL_0();I2C_Delay();

	return feedback;
}

void SENSOR_1_I2C_Ack(void)
{
	SENSOR_1_I2C_SDA_0();I2C_Delay();
	SENSOR_1_I2C_SCL_1();I2C_Delay();
	SENSOR_1_I2C_SCL_0();I2C_Delay();
	SENSOR_1_I2C_SDA_1();I2C_Delay();
}

void SENSOR_1_I2C_NAck(void)
{
	SENSOR_1_I2C_SDA_1();I2C_Delay();
	SENSOR_1_I2C_SCL_1();I2C_Delay();
	SENSOR_1_I2C_SCL_0();I2C_Delay();
}

void SENSOR_1_I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(SENSOR_1_I2C_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SENSOR_1_I2C_SCL_PIN | SENSOR_1_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SENSOR_1_I2C_GPIO_PORT, &GPIO_InitStructure);
}

void SENSOR_1_I2C_Reset(void)
{
	SENSOR_1_I2C_Stop();
}

// add one more function to check the device here.
