
#include "systick.h"

static __IO uint32_t TimeDelay;

void SYSTICK_Config(void)
{
	SysTick_Config(SystemCoreClock / 1000);		//
}

void Delay_ms(__IO uint32_t nTime)
{ 
	TimeDelay = nTime;	
	while(TimeDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimeDelay != 0x00)
	{ 
		TimeDelay--;
	}
}