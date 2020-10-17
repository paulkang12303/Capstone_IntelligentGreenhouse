#ifndef _ULN2003_28BYJ_48_H
#define	_ULN2003_28BYJ_48_H

#include "stm32f30x.h"
#include "bsp_delay.h"

#define ORANGE_PORT			GPIOB
#define ORANGE_PIN			GPIO_Pin_11
#define YELLOW_PORT			GPIOB
#define YELLOW_PIN			GPIO_Pin_12
#define PINK_PORT			GPIOB
#define PINK_PIN			GPIO_Pin_13
#define BLUE_PORT			GPIOB
#define BLUE_PIN			GPIO_Pin_14

#define	ULN2003_ORANGE_PIN_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	ULN2003_YELLOW_PIN_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	ULN2003_PINK_PIN_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
#define	ULN2003_BLUE_PIN_CLK_ENABLE()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)

#define Orange_H() 	GPIO_SetBits(ORANGE_PORT, ORANGE_PIN);
#define Orange_L() 	GPIO_ResetBits(ORANGE_PORT, ORANGE_PIN);
#define Yellow_H() 	GPIO_SetBits(YELLOW_PORT, YELLOW_PIN);
#define Yellow_L() 	GPIO_ResetBits(YELLOW_PORT, YELLOW_PIN);
#define Pink_H()  	GPIO_SetBits(PINK_PORT, PINK_PIN);
#define Pink_L()	GPIO_ResetBits(PINK_PORT, PINK_PIN);
#define Blue_H()   	GPIO_SetBits(BLUE_PORT, BLUE_PIN);
#define Blue_L()   	GPIO_ResetBits(BLUE_PORT, BLUE_PIN);

#define ULN2003_Delay_ms(n)		delay_ms(n)

void ULN2003_Config(void);
void ULN2003_OFF(void);
void ULN2003_ForwardRun(uint16_t period);
void ULN2003_BackwardRun(uint16_t period);
void ULN2003_Forword360(uint16_t period);
void ULN2003_Backword360(uint16_t period);
uint8_t ULN2003_ToTerminal(void);
uint8_t ULN2003_ToInitial(void);

#endif
