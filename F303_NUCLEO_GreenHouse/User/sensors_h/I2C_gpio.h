#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include "stm32f30x.h"

/* Define the I2C Pins for SENSOR_1 */
#define SENSOR_1_I2C_GPIO_CLK			 		RCC_AHBPeriph_GPIOA
#define SENSOR_1_I2C_GPIO_PORT					GPIOA
#define SENSOR_1_I2C_SCL_PIN					GPIO_Pin_9
#define SENSOR_1_I2C_SDA_PIN					GPIO_Pin_10

/* Set or Reset SCL for SENSOR_1*/
#define SENSOR_1_I2C_SCL_1()  GPIO_SetBits(SENSOR_1_I2C_GPIO_PORT, SENSOR_1_I2C_SCL_PIN)
#define SENSOR_1_I2C_SCL_0()  GPIO_ResetBits(SENSOR_1_I2C_GPIO_PORT, SENSOR_1_I2C_SCL_PIN)
/* Set or Reset SDA for SENSOR_1*/	
#define SENSOR_1_I2C_SDA_1()  GPIO_SetBits(SENSOR_1_I2C_GPIO_PORT, SENSOR_1_I2C_SDA_PIN)
#define SENSOR_1_I2C_SDA_0()  GPIO_ResetBits(SENSOR_1_I2C_GPIO_PORT, SENSOR_1_I2C_SDA_PIN)
/* Read SDA for SENSOR_1*/	
#define SENSOR_1_I2C_SDA_READ()  GPIO_ReadInputDataBit(SENSOR_1_I2C_GPIO_PORT, SENSOR_1_I2C_SDA_PIN)

void SENSOR_1_I2C_Start(void);
void SENSOR_1_I2C_Stop(void);
void SENSOR_1_I2C_SendByte(uint8_t _ucByte);
uint8_t SENSOR_1_I2C_ReadByte(void);
uint8_t SENSOR_1_I2C_WaitAck(void);
void SENSOR_1_I2C_Ack(void);
void SENSOR_1_I2C_NAck(void);
void SENSOR_1_I2C_Config(void);
void SENSOR_1_I2C_Reset(void);



#endif