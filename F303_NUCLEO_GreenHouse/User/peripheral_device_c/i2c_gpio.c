#include "i2c_gpio.h"

void us_delay(uint32_t uSec)
{
	while(uSec--) 
	{
		for(uint16_t count = 32/5; count > 0; count--);
	}
}

/*
 * This part is for I2C BUS_1
 */

/*I2C_BUS_1 I2C SCL & SDA setting (confirmed)*/
void I2C_BUS_1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	I2C_BUS_1_SCL_CLK_ENABLE();
	I2C_BUS_1_SDA_CLK_ENABLE();

	GPIO_InitStruct.GPIO_Pin = I2C_BUS_1_SCL_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_BUS_1_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = I2C_BUS_1_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_BUS_1_SDA_PORT, &GPIO_InitStruct);
	
	I2C_BUS_1_SDA_1();
	I2C_BUS_1_SCL_1();
}

/*I2C START Signal (confirmed)*/
uint8_t I2C_BUS_1_Start(void)
{
	I2C_BUS_1_SDA_1();us_delay(5);
	I2C_BUS_1_SCL_1();us_delay(5);
	if(I2C_BUS_1_SDA_READ()==0)	return 0;
	I2C_BUS_1_SDA_0();us_delay(5);
	if(I2C_BUS_1_SDA_READ()!=0) return 0;
	I2C_BUS_1_SCL_0();us_delay(5);
	return 1;
}
/*I2C STOP Signal (confirmed)*/
void I2C_BUS_1_Stop(void)
{
	I2C_BUS_1_SDA_0();
	I2C_BUS_1_SCL_0();us_delay(5);
	I2C_BUS_1_SCL_1();us_delay(5);
	I2C_BUS_1_SDA_1();us_delay(5);
}
/*I2C Wait Ack*/
uint8_t I2C_BUS_1_WaitAck(void)
{
	uint16_t ErrTime = 0;

	I2C_BUS_1_SDA_1();
	I2C_BUS_1_SCL_1();
	while (I2C_BUS_1_SDA_READ())	//if SDA is low, ack is received.
	{
		ErrTime++;
		if(ErrTime == 500)
			break;
	}
	if(I2C_BUS_1_SDA_READ())
	{
		I2C_BUS_1_SCL_0();us_delay(5);
		return 0;
	}
	I2C_BUS_1_SCL_0();us_delay(5);
	return 1;
}
/*I2C Send Ack*/
void I2C_BUS_1_Ack(void)
{
	I2C_BUS_1_SDA_0();us_delay(5);
	I2C_BUS_1_SCL_1();us_delay(5);	
	I2C_BUS_1_SCL_0();us_delay(5);
}
/*I2C Send NAck*/
void I2C_BUS_1_NAck(void)
{
	I2C_BUS_1_SDA_1();us_delay(5);
	I2C_BUS_1_SCL_1();us_delay(5);	
	I2C_BUS_1_SCL_0();us_delay(5);
}
/*I2C Send Byte*/
void I2C_BUS_1_SendByte(uint8_t TxByte)
{
	uint8_t i;
	
	I2C_BUS_1_SCL_0();
	for (i = 0; i < 8; i++)
	{		
		if (TxByte & 0x80)
			I2C_BUS_1_SDA_1();
		else
			I2C_BUS_1_SDA_0();
		I2C_BUS_1_SCL_1();
		us_delay(5);
		I2C_BUS_1_SCL_0();
		us_delay(5);
		TxByte <<= 1;		
	}
}
/*I2C Read Byte*/
uint8_t I2C_BUS_1_ReadByte(void)
{
	uint8_t RxByte =0 ;
	uint8_t i;
	I2C_BUS_1_SDA_1();us_delay(2);
	
	for (i = 0; i < 8; i++)
	{
		RxByte = RxByte << 1;
		I2C_BUS_1_SCL_1();
		if(I2C_BUS_1_SDA_READ())
			RxByte = RxByte | 0x01;
		us_delay(5);
		I2C_BUS_1_SCL_0();
		us_delay(5);
	}
	return RxByte;
}

uint8_t I2C_BUS_1_WriteDevide_SingleByte(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t TxByte)
{
	if(I2C_BUS_1_Start()==0)
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(SlaveAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(REGAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(TxByte);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_Stop();
	return 1;
}

uint8_t I2C_BUS_1_WriteDevice(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t const *TxByte, uint8_t length)
{
	if(I2C_BUS_1_Start()==0)
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(SlaveAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(REGAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	while(length)
	{
		I2C_BUS_1_SendByte(*TxByte++);
		if(!I2C_BUS_1_WaitAck())
		{
			I2C_BUS_1_Stop();return 0;
		}
		length--;
	}
	I2C_BUS_1_Stop();
	return 1;
}

uint8_t I2C_BUS_1_ReadDevice_Pre(uint8_t SlaveAddress, uint8_t REGAddress)
{
	if(I2C_BUS_1_Start()==0)
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(SlaveAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(REGAddress);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_Stop();
	return 1;
}

uint8_t I2C_BUS_1_ReadDevice_Post(uint8_t SlaveAddress, uint8_t *RxByte, uint8_t length)
{
	if(I2C_BUS_1_Start()==0)
	{
		I2C_BUS_1_Stop();return 0;
	}
	I2C_BUS_1_SendByte(SlaveAddress + 1);
	if(!I2C_BUS_1_WaitAck())
	{
		I2C_BUS_1_Stop();return 0;
	}
	while(length-1)
	{
		*RxByte++ = I2C_BUS_1_ReadByte();
		I2C_BUS_1_Ack();
		length--;
	}
	*RxByte = I2C_BUS_1_ReadByte();
	I2C_BUS_1_NAck();
	I2C_BUS_1_Stop();
	return 1;	
}
