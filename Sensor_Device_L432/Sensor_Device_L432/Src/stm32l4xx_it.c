

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"

extern UART_HandleTypeDef DebugUART;

void NMI_Handler(void)
{

}


void HardFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{

  while (1)
  {

  }
}


void BusFault_Handler(void)
{

  while (1)
  {

  }
}


void UsageFault_Handler(void)
{

  while (1)
  {

  }
}


void SVC_Handler(void)
{

}


void DebugMon_Handler(void)
{

}


void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{

  HAL_IncTick();

}


void USART2_IRQHandler(void)
{

  HAL_UART_IRQHandler(&DebugUART);

}


