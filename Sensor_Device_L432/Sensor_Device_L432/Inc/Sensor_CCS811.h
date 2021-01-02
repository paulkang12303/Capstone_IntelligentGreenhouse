#ifndef _SENSOR_CCS811_H
#define _SENSOR_CCS811_H

# include "stm32l4xx_hal.h"

#include "bsp_uart.h"
#include "bsp_led.h"
#include "i2c_gpio.h"

#define CCS811_Add      		0x5A<<1

#define STATUS_REG 0x00
#define MEAS_MODE_REG 0x01
#define ALG_RESULT_DATA 0x02
#define ENV_DATA 0x05
#define NTC_REG 0x06
#define THRESHOLDS 0x10
#define BASELINE 0x11
#define HW_ID_REG 0x20
#define ERROR_ID_REG 0xE0
#define APP_START_REG 0xF4
#define SW_RESET 0xFF
#define CCS_811_ADDRESS 0x5A
#define GPIO_WAKE 0x5
#define DRIVE_MODE_IDLE 0x0
#define DRIVE_MODE_1SEC 0x10
#define DRIVE_MODE_10SEC 0x20
#define DRIVE_MODE_60SEC 0x30
#define INTERRUPT_DRIVEN 0x8
#define THRESHOLDS_ENABLED 0x4

#define CCS811_CS_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define CCS811_CS_PORT				GPIOA
#define CCS811_CS_PIN				GPIO_PIN_0
#define CCS811_CS_ON()				HAL_GPIO_WritePin(CCS811_CS_PORT,CCS811_CS_PIN,GPIO_PIN_RESET)
#define CCS811_CS_OFF()				HAL_GPIO_WritePin(CCS811_CS_PORT,CCS811_CS_PIN,GPIO_PIN_SET)
#define CCS811_STEP_DELAY()			HAL_Delay(100)

typedef struct {
uint16_t eco2;
uint16_t tvoc;
uint8_t status;
uint8_t device_id;
uint8_t error_id;
uint16_t raw_data;
} CCS811_Measurement;

extern CCS811_Measurement 	CCS811;
extern uint8_t				CCS811_Data[8];

void CCS811_Config(void);
void CCS811_CS_GPIO_Config(void);
void CCS811_GetData(void);
void CCS811_ClearData(void);
#endif
