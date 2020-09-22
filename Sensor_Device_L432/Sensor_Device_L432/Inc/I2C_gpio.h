#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include "stm32l4xx_hal.h"

#define LIBRARY_FUNCTION 	0

#define I2C_WR				0
#define I2C_RD				1

/* Define the I2C Pins for SENSOR_1 */
#define I2C_BUS_1_SCL_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define I2C_BUS_1_SDA_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define I2C_BUS_1_SCL_PORT					GPIOA
#define I2C_BUS_1_SDA_PORT					GPIOA
#define I2C_BUS_1_SCL_PIN					GPIO_PIN_3
#define I2C_BUS_1_SDA_PIN					GPIO_PIN_4

#define I2C_BUS_2_SCL_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define I2C_BUS_2_SDA_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define I2C_BUS_2_SCL_PORT					GPIOA
#define I2C_BUS_2_SDA_PORT					GPIOA
#define I2C_BUS_2_SCL_PIN					GPIO_PIN_5
#define I2C_BUS_2_SDA_PIN					GPIO_PIN_6

/* if LIBRARY_FUNCTION == 1, use library function, if LIBRARY_FUNCTION == 0, use register*/
#ifdef LIBRARY_FUNCTION
	/* Set or Reset SCL for I2C_BUS_1*/
	#define I2C_BUS_1_SCL_1()  HAL_GPIO_WritePin(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN, GPIO_PIN_SET)
	#define I2C_BUS_1_SCL_0()  HAL_GPIO_WritePin(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN, GPIO_PIN_RESET)
	/* Set or Reset SDA for I2C_BUS_1*/	
	#define I2C_BUS_1_SDA_1()  HAL_GPIO_WritePin(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN, GPIO_PIN_SET)
	#define I2C_BUS_1_SDA_0()  HAL_GPIO_WritePin(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN, GPIO_PIN_RESET)
	/* Read SDA for I2C_BUS_1*/	
	#define I2C_BUS_1_SDA_READ()  HAL_GPIO_ReadPin(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
	
	/* Set or Reset SCL for I2C_BUS_2*/
	#define I2C_BUS_2_SCL_1()  HAL_GPIO_WritePin(I2C_BUS_2_SCL_PORT, I2C_BUS_2_SCL_PIN, GPIO_PIN_SET)
	#define I2C_BUS_2_SCL_0()  HAL_GPIO_WritePin(I2C_BUS_2_SCL_PORT, I2C_BUS_2_SCL_PIN, GPIO_PIN_RESET)
	/* Set or Reset SDA for I2C_BUS_2*/	
	#define I2C_BUS_2_SDA_1()  HAL_GPIO_WritePin(I2C_BUS_2_SDA_PORT, I2C_BUS_2_SDA_PIN, GPIO_PIN_SET)
	#define I2C_BUS_2_SDA_0()  HAL_GPIO_WritePin(I2C_BUS_2_SDA_PORT, I2C_BUS_2_SDA_PIN, GPIO_PIN_RESET)
	/* Read SDA for I2C_BUS_2*/	
	#define I2C_BUS_2_SDA_READ()  HAL_GPIO_ReadPin(I2C_BUS_2_SDA_PORT, I2C_BUS_2_SDA_PIN)
#else
	/* Set or Reset SCL for I2C_BUS_1*/
	#define I2C_BUS_1_SCL_1()  I2C_BUS_1_SCL_PORT->BSRRL = I2C_BUS_1_SCL_PIN				
	#define I2C_BUS_1_SCL_0()  I2C_BUS_1_SCL_PORT->BSRRH = I2C_BUS_1_SCL_PIN				
	/* Set or Reset SDA for I2C_BUS_1*/
	#define I2C_BUS_1_SDA_1()  I2C_BUS_1_SDA_PORT->BSRRL = I2C_BUS_1_SDA_PIN				
	#define I2C_BUS_1_SDA_0()  I2C_BUS_1_SDA_PORT->BSRRH = I2C_BUS_1_SDA_PIN		
	/* Read SDA for I2C_BUS_1*/
	#define I2C_BUS_1_SDA_READ()  ((I2C_BUS_1_SDA_PORT->IDR & I2C_BUS_1_SDA_PIN) != 0)	
	
	/* Set or Reset SCL for I2C_BUS_2*/
	#define I2C_BUS_2_SCL_1()  I2C_BUS_2_SCL_PORT->BSRRL = I2C_BUS_2_SCL_PIN				
	#define I2C_BUS_2_SCL_0()  I2C_BUS_2_SCL_PORT->BSRRH = I2C_BUS_2_SCL_PIN				
	/* Set or Reset SDA for I2C_BUS_2*/
	#define I2C_BUS_2_SDA_1()  I2C_BUS_2_SDA_PORT->BSRRL = I2C_BUS_2_SDA_PIN				
	#define I2C_BUS_2_SDA_0()  I2C_BUS_2_SDA_PORT->BSRRH = I2C_BUS_2_SDA_PIN		
	/* Read SDA for I2C_BUS_2*/
	#define I2C_BUS_2_SDA_READ()  ((I2C_BUS_2_SDA_PORT->IDR & I2C_BUS_2_SDA_PIN) != 0)	
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

void 	I2C_BUS_2_Config(void);
void 	I2C_BUS_2_Start(void);
void 	I2C_BUS_2_Stop(void);
void 	I2C_BUS_2_SendByte(uint8_t _ucByte);
uint8_t I2C_BUS_2_ReadByte(void);
uint8_t I2C_BUS_2_WaitAck(void);
void 	I2C_BUS_2_Ack(void);
void 	I2C_BUS_2_NAck(void);
void 	I2C_BUS_2_Reset(void);
uint8_t I2C_BUS_2_CheckDevice(uint8_t Address);

#endif
