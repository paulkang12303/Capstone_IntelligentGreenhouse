
#include "bsp_usart.h"


static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void DEBUG_USART_Config(void)
{
  DEBUG_USART_CLK_ENABLE();
  DEBUG_USART_GPIO_CLK_ENABLE();
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_PinAFConfig(DEBUG_USART_TX_PORT,DEBUG_USART_TX_PIN_SOURCE,GPIO_AF_7);
  GPIO_PinAFConfig(DEBUG_USART_RX_PORT,DEBUG_USART_RX_PIN_SOURCE,GPIO_AF_7);
	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
  GPIO_Init(DEBUG_USART_TX_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;	
  GPIO_Init(DEBUG_USART_RX_PORT, &GPIO_InitStructure);	

  USART_InitTypeDef USART_InitStructure;
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART_ID, &USART_InitStructure); 
  
//  NVIC_Configuration();
//  
//  USART_ITConfig(DEBUG_USART_ID, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(DEBUG_USART_ID, ENABLE);
}





static void NVIC_COM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = COM_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void COM_USART_Config(void)
{
  COM_USART_CLK_ENABLE();
  COM_USART_GPIO_CLK_ENABLE();
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_PinAFConfig(COM_USART_TX_PORT,COM_USART_TX_PIN_SOURCE,GPIO_AF_5);
  GPIO_PinAFConfig(COM_USART_RX_PORT,COM_USART_RX_PIN_SOURCE,GPIO_AF_5);
	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_USART_TX_PIN;
  GPIO_Init(COM_USART_TX_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_USART_RX_PIN;	
  GPIO_Init(COM_USART_RX_PORT, &GPIO_InitStructure);	

  USART_InitTypeDef USART_InitStructure;
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(COM_USART_ID, &USART_InitStructure); 
  
  NVIC_COM_Configuration();
  
  USART_ITConfig(COM_USART_ID, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(COM_USART_ID, ENABLE);
}




int fputc(int ch, FILE *f)
{

		USART_SendData(COM_USART_ID, (uint8_t) ch);
		
		while (USART_GetFlagStatus(COM_USART_ID, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
	
		while (USART_GetFlagStatus(COM_USART_ID, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(COM_USART_ID);
}

//int fputc(int ch, FILE *f)
//{

//		USART_SendData(DEBUG_USART_ID, (uint8_t) ch);
//		
//		while (USART_GetFlagStatus(DEBUG_USART_ID, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

//int fgetc(FILE *f)
//{
//	
//		while (USART_GetFlagStatus(DEBUG_USART_ID, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USART_ID);
//}

