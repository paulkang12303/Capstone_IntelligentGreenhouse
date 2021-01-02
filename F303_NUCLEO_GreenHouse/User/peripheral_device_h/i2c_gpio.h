#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include "stm32f30x.h"

#define LIBRARY_FUNCTION 	0

#define I2C_WR				0
#define I2C_RD				1

/* Define the I2C Pins for BUS_1 (Tested) */
#define I2C_BUS_1_SCL_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)
#define I2C_BUS_1_SDA_CLK_ENABLE()			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE)
#define I2C_BUS_1_SCL_PORT					GPIOC
#define I2C_BUS_1_SDA_PORT					GPIOC
#define I2C_BUS_1_SCL_PIN					GPIO_Pin_8
#define I2C_BUS_1_SDA_PIN					GPIO_Pin_9

/* if LIBRARY_FUNCTION == 1, use library function, if LIBRARY_FUNCTION == 0, use register*/
#ifdef LIBRARY_FUNCTION
	/* Set or Reset SCL for I2C_BUS_1*/
	#define I2C_BUS_1_SCL_1()  GPIO_SetBits(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN)
	#define I2C_BUS_1_SCL_0()  GPIO_ResetBits(I2C_BUS_1_SCL_PORT, I2C_BUS_1_SCL_PIN)
	/* Set or Reset SDA for I2C_BUS_1*/	
	#define I2C_BUS_1_SDA_1()  GPIO_SetBits(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
	#define I2C_BUS_1_SDA_0()  GPIO_ResetBits(I2C_BUS_1_SDA_PORT, I2C_BUS_1_SDA_PIN)
	/* Read SDA for I2C_BUS_1*/	
	#define I2C_BUS_1_SDA_READ()  ((I2C_BUS_1_SDA_PORT->IDR & I2C_BUS_1_SDA_PIN) != 0)	
	
#else
	/* Set or Reset SCL for I2C_BUS_1*/
	#define I2C_BUS_1_SCL_1()  I2C_BUS_1_SCL_PORT->BSRRL = I2C_BUS_1_SCL_PIN				
	#define I2C_BUS_1_SCL_0()  I2C_BUS_1_SCL_PORT->BSRRH = I2C_BUS_1_SCL_PIN				
	/* Set or Reset SDA for I2C_BUS_1*/
	#define I2C_BUS_1_SDA_1()  I2C_BUS_1_SDA_PORT->BSRRL = I2C_BUS_1_SDA_PIN				
	#define I2C_BUS_1_SDA_0()  I2C_BUS_1_SDA_PORT->BSRRH = I2C_BUS_1_SDA_PIN		
	/* Read SDA for I2C_BUS_1*/
	#define I2C_BUS_1_SDA_READ()  ((I2C_BUS_1_SDA_PORT->IDR & I2C_BUS_1_SDA_PIN) != 0)		

#endif

void us_delay(uint32_t uSec);

void 	I2C_BUS_1_Config(void);
uint8_t I2C_BUS_1_Start(void);
void 	I2C_BUS_1_Stop(void);
void 	I2C_BUS_1_SendByte(uint8_t _ucByte);
uint8_t I2C_BUS_1_ReadByte(void);
uint8_t I2C_BUS_1_WaitAck(void);
void 	I2C_BUS_1_Ack(void);
void 	I2C_BUS_1_NAck(void);
void 	I2C_BUS_1_Reset(void);
uint8_t I2C_BUS_1_WriteDevide_SingleByte(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t TxByte);
uint8_t I2C_BUS_1_WriteDevice(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t const *TxByte, uint8_t length);
uint8_t I2C_BUS_1_ReadDevice_Pre(uint8_t SlaveAddress, uint8_t REGAddress);
uint8_t I2C_BUS_1_ReadDevice_Post(uint8_t SlaveAddress, uint8_t *RxByte, uint8_t length);


#endif
