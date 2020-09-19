#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include "stm32f30x.h"

#define LIBRARY_FUNCTION 	0

#define I2C_WR				0
#define I2C_RD				1

/* Define the I2C Pins for SENSOR_1 */
#define I2C_BUS_1_SCL_PORT					GPIOA
#define I2C_BUS_1_SDA_PORT					GPIOA
#define I2C_BUS_1_SCL_PIN					GPIO_Pin_9
#define I2C_BUS_1_SDA_PIN					GPIO_Pin_10
#define I2C_BUS_1_SCL_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)
#define I2C_BUS_1_SDA_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)

/* if LIBRARY_FUNCTION == 1, use library function, if LIBRARY_FUNCTION == 0, use register*/
#ifdef LIBRARY_FUNCTION
	/* Set or Reset SCL for SENSOR_1*/
	#define I2C_BUS_1_SCL_1()  GPIO_SetBits(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN)
	#define I2C_BUS_1_SCL_0()  GPIO_ResetBits(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN)
	/* Set or Reset SDA for SENSOR_1*/	
	#define I2C_BUS_1_SDA_1()  GPIO_SetBits(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
	#define I2C_BUS_1_SDA_0()  GPIO_ResetBits(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
	/* Read SDA for SENSOR_1*/	
	#define I2C_BUS_1_SDA_READ()  GPIO_ReadInputDataBit(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
#else
	/* Set or Reset SCL for SENSOR_1*/
	#define I2C_BUS_1_SCL_1()  I2C_BUS_1_SCL_PORT->BSRRL = I2C_BUS_1_SCL_PIN				
	#define I2C_BUS_1_SCL_0()  I2C_BUS_1_SCL_PORT->BSRRH = I2C_BUS_1_SCL_PIN				
	/* Set or Reset SDA for SENSOR_1*/
	#define I2C_BUS_1_SDA_1()  I2C_BUS_1_SDA_PORT->BSRRL = I2C_BUS_1_SDA_PIN				
	#define I2C_BUS_1_SDA_0()  I2C_BUS_1_SDA_PORT->BSRRH = I2C_BUS_1_SDA_PIN		
	/* Read SDA for SENSOR_1*/
	#define I2C_BUS_1_SDA_READ()  ((I2C_BUS_1_SDA_PORT->IDR & I2C_BUS_1_SDA_PIN) != 0)	
#endif

void 	I2C_BUS_1_Config(void);
void 	I2C_BUS_1_Start(void);
void 	I2C_BUS_1_Stop(void);
void 	I2C_BUS_1_SendByte(uint8_t _ucByte);
uint8_t I2C_BUS_1_ReadByte(void);
uint8_t I2C_BUS_1_WaitAck(void);
void 	I2C_BUS_1_Ack(void);
void 	I2C_BUS_1_NAck(void);
void 	I2C_BUS_1_Reset(void);
uint8_t I2C_BUS_1_CheckDevice(uint8_t Address);



#endif
