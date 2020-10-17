#include "manual_controller.h"
  

void ManualControl_KEY_Config(void)
{	
	KEY_MAN_EN_CLK_ENABLE();
	KEY_LMP_ON_CLK_ENABLE();
	KEY_SHT_UP_CLK_ENABLE();
	KEY_SHT_DW_CLK_ENABLE();
	KEY_CLR_ON_CLK_ENABLE();
	KEY_HTR_ON_CLK_ENABLE();
	KEY_CO2_ON_CLK_ENABLE();
	KEY_FAN_ON_CLK_ENABLE();
	KEY_SPR_ON_CLK_ENABLE();
	KEY_ALM_ON_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	//setting for KEY_MAN_EN
	GPIO_InitStruct.GPIO_Pin = KEY_MAN_EN_PIN;
	GPIO_Init(KEY_MAN_EN_PORT, &GPIO_InitStruct);
	//setting for KEY_LMP_ON
	GPIO_InitStruct.GPIO_Pin = KEY_LMP_ON_PIN;
	GPIO_Init(KEY_LMP_ON_PORT, &GPIO_InitStruct);
	//setting for KEY_SHT_UP
	GPIO_InitStruct.GPIO_Pin = KEY_SHT_UP_PIN;
	GPIO_Init(KEY_SHT_UP_PORT, &GPIO_InitStruct);
	//setting for KEY_SHT_DW
	GPIO_InitStruct.GPIO_Pin = KEY_SHT_DW_PIN;
	GPIO_Init(KEY_SHT_DW_PORT, &GPIO_InitStruct);
	//setting for KEY_SHT_DW
	GPIO_InitStruct.GPIO_Pin = KEY_SHT_DW_PIN;
	GPIO_Init(KEY_SHT_DW_PORT, &GPIO_InitStruct);
	//setting for KEY_CLR_ON
	GPIO_InitStruct.GPIO_Pin = KEY_CLR_ON_PIN;
	GPIO_Init(KEY_CLR_ON_PORT, &GPIO_InitStruct);	
	//setting for KEY_HTR_ON
	GPIO_InitStruct.GPIO_Pin = KEY_HTR_ON_PIN;
	GPIO_Init(KEY_HTR_ON_PORT, &GPIO_InitStruct);	
	//setting for KEY_CO2_ON
	GPIO_InitStruct.GPIO_Pin = KEY_CO2_ON_PIN;
	GPIO_Init(KEY_CO2_ON_PORT, &GPIO_InitStruct);	
	//setting for KEY_FAN_ON
	GPIO_InitStruct.GPIO_Pin = KEY_FAN_ON_PIN;
	GPIO_Init(KEY_FAN_ON_PORT, &GPIO_InitStruct);	
	//setting for KEY_SPR_ON
	GPIO_InitStruct.GPIO_Pin = KEY_SPR_ON_PIN;
	GPIO_Init(KEY_SPR_ON_PORT, &GPIO_InitStruct);		
	//setting for KEY_ALM_ON
	GPIO_InitStruct.GPIO_Pin = KEY_ALM_ON_PIN;
	GPIO_Init(KEY_ALM_ON_PORT, &GPIO_InitStruct);	
}


uint8_t ManualControl_Key_Scan_Continue(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
}
