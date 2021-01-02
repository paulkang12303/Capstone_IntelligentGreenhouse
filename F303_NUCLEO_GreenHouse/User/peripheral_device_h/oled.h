#ifndef _OLED_H
#define _OLED_H

#include "stm32f30x.h"
#include "i2c_gpio.h"

//#define OLED_I2C_WriteDevide_SingleByte(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t TxByte)	I2C_BUS_1_WriteDevide_SingleByte(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t TxByte)

void WriteCmd(unsigned char OLED_Command);
void WriteDat(unsigned char OLED_Command);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void welcome_info(void);


#endif


