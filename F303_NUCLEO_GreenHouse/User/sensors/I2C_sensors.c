#include "I2C_sensors.h"
#include "I2C_gpio.h"

#include "bsp_usart.h"

void SHT30_ReadData( void )
{
	float g_tmp,g_frh;
	unsigned short buff[6];
	unsigned short tem=0,rh=0;
	
	I2C_BUS_1_Start();
	I2C_BUS_1_SendByte(0x88);
	I2C_BUS_1_WaitAck();
	I2C_BUS_1_SendByte(0x2C);
	I2C_BUS_1_WaitAck();
	I2C_BUS_1_SendByte(0x06);
	I2C_BUS_1_WaitAck();
	
	I2C_BUS_1_Start();
	
	I2C_BUS_1_SendByte(0x89);
	I2C_BUS_1_WaitAck();
	
	buff[0]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	buff[1]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	buff[2]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	buff[3]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	buff[4]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	buff[5]=I2C_BUS_1_ReadByte();
	I2C_BUS_1_Ack();
	
	I2C_BUS_1_Stop();
	
	tem=((buff[0]<<8)|buff[1]);
	rh=((buff[3]<<8)|buff[4]);
	
	g_tmp=175.0*(float)tem/65536.0-45.0;
	g_frh=100.0*(float)rh/65536.0;
	
	printf(" %f , %f \r\n",g_tmp,g_frh);
}
