
#include "stm32f30x_it.h"
#include "bsp_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern xQueueHandle Queue_Handle_RxData;


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


//void SVC_Handler(void)
//{
//}


void DebugMon_Handler(void)
{
}


//void PendSV_Handler(void)
//{
//}

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}


void UART4_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	uint8_t Rx_Byte;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{	
		Rx_Byte = USART_ReceiveData(UART4);
		xQueueSendToBackFromISR(Queue_Handle_RxData, &Rx_Byte, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	} 
}

//void USART2_IRQHandler(void)
//{
//	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//	uint8_t Rx_Byte;

//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{	
//		Rx_Byte = USART_ReceiveData(USART2);
//		xQueueSendToBackFromISR(Queue_Handle_RxData, &Rx_Byte, &xHigherPriorityTaskWoken);
//		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//	} 
//}
