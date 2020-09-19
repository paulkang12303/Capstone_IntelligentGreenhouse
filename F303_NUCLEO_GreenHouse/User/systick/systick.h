#ifndef _BSP_SYSTICK_H
#define	_BSP_SYSTICK_H

#include "stm32f30x.h"

void SYSTICK_Config(void);
void Delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif 
