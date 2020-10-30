#include "timer.h"

TIM_HandleTypeDef SysMainTimer;

void SystemMainTimer_Config(void)
{
	TIM_ClockConfigTypeDef 	sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef 	sMasterConfig = {0};

	SysMainTimer.Instance = TIM2;
	SysMainTimer.Init.Prescaler = 3200-1;
	SysMainTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	SysMainTimer.Init.Period = 20000-1;
	SysMainTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	SysMainTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  
	HAL_TIM_Base_Init(&SysMainTimer);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	
	HAL_TIM_ConfigClockSource(&SysMainTimer, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&SysMainTimer, &sMasterConfig);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM2)
  {
    __HAL_RCC_TIM2_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
}

