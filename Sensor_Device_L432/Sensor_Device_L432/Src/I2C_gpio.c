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

/*I2C_BUS_3 I2C SCL & SDA setting (confirmed)*/
void I2C_BUS_3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	I2C_BUS_3_SCL_CLK_ENABLE();
	I2C_BUS_3_SDA_CLK_ENABLE();

	GPIO_InitStruct.Pin = I2C_BUS_3_SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_3_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = I2C_BUS_3_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_BUS_3_SDA_PORT, &GPIO_InitStruct);
	
	I2C_BUS_3_Stop();
}

/*I2C START Signal (confirmed)*/
void I2C_BUS_3_Start(void)
{
	I2C_BUS_3_SCL_1();
	I2C_BUS_3_SDA_1();us_delay(2);
	I2C_BUS_3_SDA_0();us_delay(2);
	I2C_BUS_3_SCL_0();us_delay(2);
}
/*I2C STOP Signal (confirmed)*/
void I2C_BUS_3_Stop(void)
{
	I2C_BUS_3_SCL_0();
	I2C_BUS_3_SDA_0();us_delay(2);
	I2C_BUS_3_SCL_1();us_delay(2);
	I2C_BUS_3_SDA_1();us_delay(2);
}
/*I2C Wait Ack*/
uint8_t I2C_BUS_3_WaitAck(void)
{
	uint8_t ErrTime = 0;

	I2C_BUS_3_SDA_1();us_delay(2);
	I2C_BUS_3_SCL_1();us_delay(2);
	while (I2C_BUS_3_SDA_READ())	//if SDA is low, ack is received.
	{
		ErrTime++;
		if(ErrTime > 250)
		{
			return 1;
		}
	}
	I2C_BUS_3_SCL_0();us_delay(2);
	
	return 0;
}
/*I2C Send Ack*/
void I2C_BUS_3_Ack(void)
{
	I2C_BUS_3_SCL_0();
	I2C_BUS_3_SDA_0();us_delay(2);
	I2C_BUS_3_SCL_1();us_delay(2);	
	I2C_BUS_3_SCL_0();us_delay(2);
	I2C_BUS_3_SDA_1();us_delay(2);
}
/*I2C Send NAck*/
void I2C_BUS_3_NAck(void)
{
	I2C_BUS_3_SCL_0();
	I2C_BUS_3_SDA_1();us_delay(2);
	I2C_BUS_3_SCL_1();us_delay(2);	
	I2C_BUS_3_SCL_0();us_delay(2);
}
/*I2C Send Byte*/
void I2C_BUS_3_SendByte(uint8_t TxByte)
{
	uint8_t i;
	
	I2C_BUS_3_SCL_0();
	for (i = 0; i < 8; i++)
	{		
		if ((TxByte & 0x80)>>7)
		{
			I2C_BUS_3_SDA_1();
		}
		else
		{
			I2C_BUS_3_SDA_0();
		}
		TxByte <<= 1;
		us_delay(2);
		I2C_BUS_3_SCL_1();
		us_delay(2);
		I2C_BUS_3_SCL_0();
		us_delay(2);		
	}
}
/*I2C Read Byte*/
uint8_t I2C_BUS_3_ReadByte(void)
{
	uint8_t RxByte =0 ;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		I2C_BUS_3_SCL_0();us_delay(2);
		I2C_BUS_3_SCL_1();
		RxByte = RxByte << 1;
		if(I2C_BUS_3_SDA_READ())
		{
			RxByte++;
		}
		us_delay(2);
	}
	return RxByte;
}


uint8_t I2C_BUS_3_CheckDevice(uint8_t Address)
{
	uint8_t status;
	
	I2C_BUS_3_Start();
	I2C_BUS_3_SendByte( Address<<1 | I2C_WR );
	status = I2C_BUS_3_WaitAck();
	I2C_BUS_3_Stop();
	
	return status;
}


