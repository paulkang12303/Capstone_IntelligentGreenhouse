#include "I2C_gpio.h"

static void I2C_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 60; i++);
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
/*SENSOR_1 I2C SCL & SDA setting*/
void I2C_BUS_1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(I2C_BUS_1_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2C_BUS_1_SCL_PIN | I2C_BUS_1_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_BUS_1_GPIO_PORT, &GPIO_InitStructure);
}
/*SENSOR_1 I2C Reset*/
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


	
