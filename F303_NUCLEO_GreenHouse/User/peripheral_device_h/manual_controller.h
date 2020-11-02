#ifndef _MANUAL_CONTROLLER_H
#define	_MANUAL_CONTROLLER_H

#include "stm32f30x.h"

#define		MANUAL_SCAN_TIME			200
#define		STATUS_OFF					0
#define		STATUS_ON					1
#define		MOTOR_INIT_POSITION			0
#define		MOTOT_END_POSITION			1
#define		AC_CLOSE					0
#define		AC_HEATER					1
#define		AC_COOLER					2

#define 	KEY_MAN_EN_PORT 			GPIOB
#define		KEY_MAN_EN_PIN				GPIO_Pin_0
#define 	KEY_LMP_ON_PORT 			GPIOB
#define		KEY_LMP_ON_PIN				GPIO_Pin_1
#define 	KEY_SHT_UP_PORT 			GPIOB
#define		KEY_SHT_UP_PIN				GPIO_Pin_2
#define 	KEY_SHT_DW_PORT 			GPIOB
#define		KEY_SHT_DW_PIN				GPIO_Pin_3
#define 	KEY_CLR_ON_PORT 			GPIOB
#define		KEY_CLR_ON_PIN				GPIO_Pin_4
#define 	KEY_HTR_ON_PORT 			GPIOB
#define		KEY_HTR_ON_PIN				GPIO_Pin_5
#define 	KEY_CO2_ON_PORT 			GPIOB
#define		KEY_CO2_ON_PIN				GPIO_Pin_6
#define 	KEY_FAN_ON_PORT 			GPIOB
#define		KEY_FAN_ON_PIN				GPIO_Pin_7
#define 	KEY_SPR_ON_PORT 			GPIOB
#define		KEY_SPR_ON_PIN				GPIO_Pin_8
#define 	KEY_ALM_ON_PORT 			GPIOB
#define		KEY_ALM_ON_PIN				GPIO_Pin_9


#define	KEY_MAN_EN_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_LMP_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)	
#define	KEY_SHT_UP_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_SHT_DW_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_CLR_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_HTR_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_CO2_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_FAN_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_SPR_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	KEY_ALM_ON_CLK_ENABLE();		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

void ManualControl_KEY_Config(void);
uint8_t ManualControl_Key_Scan_Continue(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
#endif
