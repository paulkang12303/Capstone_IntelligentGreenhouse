
#include "bsp_key.h"
  

void DEBUG_KEY_Config(void)
{		
	KEY1_GPIO_CLK_ENABLE();
	KEY2_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	//setting for KEY1
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
	//setting for KEY2
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

uint8_t Debug_Key_Scan_Single(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
	{	 
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);   
		return 0;	 
	}
	else
		return 1;
}

uint8_t Debug_Key_Scan_Continue(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
}