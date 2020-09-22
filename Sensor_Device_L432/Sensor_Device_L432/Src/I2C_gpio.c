#include "I2C_gpio.h"

void us_delay(uint32_t uSec)
{
	while(uSec--) 
	{
		for(uint16_t count = 32/5; count > 0; count--);
	}
}

/*I2C_BUS_1 I2C SCL & SDA setting (confirmed)*/
void I2C_BUS_1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	I2C_BUS_1_SCL_CLK_ENABLE();
	I2C_BUS_1_SDA_CLK_ENABLE();

	GPIO_InitStruct.Pin = I2C_BUS_1_SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_1_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = I2C_BUS_1_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_1_SDA_PORT, &GPIO_InitStruct);
	
	I2C_BUS_1_Stop();
}

/*I2C START Signal (confirmed)*/
void I2C_BUS_1_Start(void)
{
	I2C_BUS_1_SCL_1();
	I2C_BUS_1_SDA_1();us_delay(2);
	I2C_BUS_1_SDA_0();us_delay(2);
	I2C_BUS_1_SCL_0();us_delay(2);
}
/*I2C STOP Signal (confirmed)*/
void I2C_BUS_1_Stop(void)
{
	I2C_BUS_1_SCL_0();
	I2C_BUS_1_SDA_0();us_delay(2);
	I2C_BUS_1_SCL_1();us_delay(2);
	I2C_BUS_1_SDA_1();us_delay(2);
}
/*I2C Wait Ack*/
uint8_t I2C_BUS_1_WaitAck(void)
{
	uint8_t ErrTime = 0;

	I2C_BUS_1_SDA_1();us_delay(2);
	I2C_BUS_1_SCL_1();us_delay(2);
	while (I2C_BUS_1_SDA_READ())	//if SDA is low, ack is received.
	{
		ErrTime++;
		if(ErrTime > 250)
		{
			return 1;
		}
	}
	I2C_BUS_1_SCL_0();us_delay(2);
	
	return 0;
}
/*I2C Send Ack*/
void I2C_BUS_1_Ack(void)
{
	I2C_BUS_1_SCL_0();
	I2C_BUS_1_SDA_0();us_delay(2);
	I2C_BUS_1_SCL_1();us_delay(2);	
	I2C_BUS_1_SCL_0();us_delay(2);
	I2C_BUS_1_SDA_1();us_delay(2);
}
/*I2C Send NAck*/
void I2C_BUS_1_NAck(void)
{
	I2C_BUS_1_SCL_0();
	I2C_BUS_1_SDA_1();us_delay(2);
	I2C_BUS_1_SCL_1();us_delay(2);	
	I2C_BUS_1_SCL_0();us_delay(2);
}
/*I2C Send Byte*/
void I2C_BUS_1_SendByte(uint8_t TxByte)
{
	uint8_t i;
	
	I2C_BUS_1_SCL_0();
	for (i = 0; i < 8; i++)
	{		
		if ((TxByte & 0x80)>>7)
		{
			I2C_BUS_1_SDA_1();
		}
		else
		{
			I2C_BUS_1_SDA_0();
		}
		TxByte <<= 1;
		us_delay(2);
		I2C_BUS_1_SCL_1();
		us_delay(2);
		I2C_BUS_1_SCL_0();
		us_delay(2);		
	}
}
/*I2C Read Byte*/
uint8_t I2C_BUS_1_ReadByte(void)
{
	uint8_t RxByte =0 ;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		I2C_BUS_1_SCL_0();us_delay(2);
		I2C_BUS_1_SCL_1();
		RxByte = RxByte << 1;
		if(I2C_BUS_1_SDA_READ())
		{
			RxByte++;
		}
		us_delay(2);
	}
	return RxByte;
}


uint8_t I2C_BUS_1_CheckDevice(uint8_t Address)
{
	uint8_t status;
	
	I2C_BUS_1_Start();
	I2C_BUS_1_SendByte( Address<<1 | I2C_WR );
	status = I2C_BUS_1_WaitAck();
	I2C_BUS_1_Stop();
	
	return status;
}


/*I2C_BUS_2 I2C SCL & SDA setting (confirmed)*/
void I2C_BUS_2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	I2C_BUS_2_SCL_CLK_ENABLE();
	I2C_BUS_2_SDA_CLK_ENABLE();

	GPIO_InitStruct.Pin = I2C_BUS_2_SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_2_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = I2C_BUS_2_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_2_SDA_PORT, &GPIO_InitStruct);
	
	I2C_BUS_2_Stop();
}

/*I2C START Signal (confirmed)*/
void I2C_BUS_2_Start(void)
{
	I2C_BUS_2_SCL_1();
	I2C_BUS_2_SDA_1();us_delay(2);
	I2C_BUS_2_SDA_0();us_delay(2);
	I2C_BUS_2_SCL_0();us_delay(2);
}
/*I2C STOP Signal (confirmed)*/
void I2C_BUS_2_Stop(void)
{
	I2C_BUS_2_SCL_0();
	I2C_BUS_2_SDA_0();us_delay(2);
	I2C_BUS_2_SCL_1();us_delay(2);
	I2C_BUS_2_SDA_1();us_delay(2);
}
/*I2C Wait Ack*/
uint8_t I2C_BUS_2_WaitAck(void)
{
	uint8_t ErrTime = 0;

	I2C_BUS_2_SDA_1();us_delay(2);
	I2C_BUS_2_SCL_1();us_delay(2);
	while (I2C_BUS_2_SDA_READ())	//if SDA is low, ack is received.
	{
		ErrTime++;
		if(ErrTime > 250)
		{
			return 1;
		}
	}
	I2C_BUS_2_SCL_0();us_delay(2);
	
	return 0;
}
/*I2C Send Ack*/
void I2C_BUS_2_Ack(void)
{
	I2C_BUS_2_SCL_0();
	I2C_BUS_2_SDA_0();us_delay(2);
	I2C_BUS_2_SCL_1();us_delay(2);	
	I2C_BUS_2_SCL_0();us_delay(2);
	I2C_BUS_2_SDA_1();us_delay(2);
}
/*I2C Send NAck*/
void I2C_BUS_2_NAck(void)
{
	I2C_BUS_2_SCL_0();
	I2C_BUS_2_SDA_1();us_delay(2);
	I2C_BUS_2_SCL_1();us_delay(2);	
	I2C_BUS_2_SCL_0();us_delay(2);
}
/*I2C Send Byte*/
void I2C_BUS_2_SendByte(uint8_t TxByte)
{
	uint8_t i;
	
	I2C_BUS_2_SCL_0();
	for (i = 0; i < 8; i++)
	{		
		if ((TxByte & 0x80)>>7)
		{
			I2C_BUS_2_SDA_1();
		}
		else
		{
			I2C_BUS_2_SDA_0();
		}
		TxByte <<= 1;
		us_delay(2);
		I2C_BUS_2_SCL_1();
		us_delay(2);
		I2C_BUS_2_SCL_0();
		us_delay(2);		
	}
}
/*I2C Read Byte*/
uint8_t I2C_BUS_2_ReadByte(void)
{
	uint8_t RxByte =0 ;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		I2C_BUS_2_SCL_0();us_delay(2);
		I2C_BUS_2_SCL_1();
		RxByte = RxByte << 1;
		if(I2C_BUS_2_SDA_READ())
		{
			RxByte++;
		}
		us_delay(2);
	}
	return RxByte;
}


uint8_t I2C_BUS_2_CheckDevice(uint8_t Address)
{
	uint8_t status;
	
	I2C_BUS_2_Start();
	I2C_BUS_2_SendByte( Address<<1 | I2C_WR );
	status = I2C_BUS_2_WaitAck();
	I2C_BUS_2_Stop();
	
	return status;
}