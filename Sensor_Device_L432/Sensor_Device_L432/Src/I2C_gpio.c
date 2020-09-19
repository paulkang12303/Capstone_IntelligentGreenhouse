#include "I2C_gpio.h"

static void I2C_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 30; i++);
}

/*I2C_BUS_1 I2C SCL & SDA setting*/
void I2C_BUS_1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	I2C_BUS_1_SCL_CLK_ENABLE();
	I2C_BUS_1_SDA_CLK_ENABLE();

	GPIO_InitStruct.Pin = I2C_BUS_1_SCL_PIN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  	
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(I2C_BUS_1_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = I2C_BUS_1_SDA_PIN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  	
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(I2C_BUS_1_SDA_PORT, &GPIO_InitStruct);
}

/*I2C START Signal*/
void I2C_BUS_1_Start(void)
{
	I2C_BUS_1_SDA_1();
	I2C_BUS_1_SCL_1();I2C_Delay();
	I2C_BUS_1_SDA_0();I2C_Delay();
	I2C_BUS_1_SCL_0();
}
/*I2C STOP Signal*/
void I2C_BUS_1_Stop(void)
{
	I2C_BUS_1_SCL_0();
	I2C_BUS_1_SDA_0();I2C_Delay();
	I2C_BUS_1_SCL_1();I2C_Delay();
	I2C_BUS_1_SDA_1();
}
/*I2C Send Byte*/
void I2C_BUS_1_SendByte(uint8_t TxByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{		
		if (TxByte & 0x80)
		{
			I2C_BUS_1_SDA_1();
		}
		else
		{
			I2C_BUS_1_SDA_0();
		}
		I2C_Delay();
		I2C_BUS_1_SCL_1();
		I2C_Delay();	
		I2C_BUS_1_SCL_0();
		I2C_Delay();
		if (i == 7)
		{
			I2C_BUS_1_SDA_1(); 
		}
		TxByte <<= 1;
	}
}
/*I2C Read Byte*/
uint8_t I2C_BUS_1_ReadByte(void)
{
	uint8_t RxByte =0 ;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		RxByte <<= 1;
		I2C_BUS_1_SCL_1();I2C_Delay();
		if (I2C_BUS_1_SDA_READ())
		{
			RxByte++;
		}
		I2C_BUS_1_SCL_0();I2C_Delay();
	}
	return RxByte;
}
/*I2C Wait Ack*/
uint8_t I2C_BUS_1_WaitAck(void)
{
	uint8_t feedback;

	I2C_BUS_1_SDA_1();I2C_Delay();
	I2C_BUS_1_SCL_1();I2C_Delay();
	if (I2C_BUS_1_SDA_READ())	
	{
		feedback = 1;
	}
	else
	{
		feedback = 0;
	}
	I2C_BUS_1_SCL_0();I2C_Delay();

	return feedback;
}
/*I2C Send Ack*/
void I2C_BUS_1_Ack(void)
{
	I2C_BUS_1_SDA_0();I2C_Delay();
	I2C_BUS_1_SCL_1();I2C_Delay();	
	I2C_BUS_1_SCL_0();I2C_Delay();
	I2C_BUS_1_SDA_1();//release the I2C bus
}
/*I2C Send NAck*/
void I2C_BUS_1_NAck(void)
{
	I2C_BUS_1_SDA_1();I2C_Delay();
	I2C_BUS_1_SCL_1();I2C_Delay();	
	I2C_BUS_1_SCL_0();I2C_Delay();
	I2C_BUS_1_SDA_1();//release the I2C bus
}
/*I2C_BUS_1 I2C Reset*/
void I2C_BUS_1_Reset(void)
{
	I2C_BUS_1_Stop();
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


