


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"

#include "timer.h"
#include "exti.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "sensor_CCS811.h"
#include "sensor_bh1750.h"
#include "sensor_hdc1080.h"
#include "sensor_soilmoisture.h"

#define	TIME_TRIGGER_ENABLE 0	//1 means measurement is drived by timer, 0 means by EXTI

void SystemClock_Config(void);
void RUN_TRIGGER_Config(void);
void PeripheralSensor_Config(void);

uint8_t Main_Task_ID = MAIN_TASK__NONE;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	
	DEBUG_LED_Config();
	DEBUG_UART_Config();
	
	PeripheralSensor_Config();

	RUN_TRIGGER_Config();
	
	while (1)
	{
		switch (Main_Task_ID)
		{
			case MAIN_TASK__NONE:
				main_task__idle();
				break;
			case MAIN_TASK__DEAL_WITH_TRIGGER:
				main_task__deal_with_trigger();
				Main_Task_ID = MAIN_TASK__MANAGE_DATA;
				break;
			case MAIN_TASK__MANAGE_DATA:
				main_task__manage_data();
				Main_Task_ID = MAIN_TASK__NONE;
				break;
		}
	}
}



void PeripheralSensor_Config(void)
{
	BH1750_Config();
	HDC1080_Config();
	CCS811_Config();
	SoilMoisture_Config();
}

void RUN_TRIGGER_Config(void)
{
	if (TIME_TRIGGER_ENABLE == 1)
	{
		SystemMainTimer_Config();
		HAL_TIM_Base_Start_IT(&SysMainTimer);
	}
	else
	{
		EXTI_PA11_Config();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Main_Task_ID = MAIN_TASK__DEAL_WITH_TRIGGER;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == RUN_TRIGGER_PIN)
	{
		if(HAL_GPIO_ReadPin(RUN_TRIGGER_PORT,RUN_TRIGGER_PIN) == 1)
		{
			Main_Task_ID = MAIN_TASK__DEAL_WITH_TRIGGER;
		}
	}
}

void main_task__idle(void)
{
	;
}

void main_task__deal_with_trigger(void)
{
	DEBUG_LED_ON();
	
	BH1750_Start();
	HDC1080_Start();
	CCS811_ClearData();
	CCS811_GetData();
	SoilMoisture_GetData();
	
	HAL_Delay(150);
	
	BH1750_ReadData(BH1750_Data);
	BH1750_GetResult(BH1750_Data,&BH1750_Illumination);
	BH1750_ConvertResultToInteger(&BH1750_Illumination,&BH1750_Illumination_TX);

	HDC1080_ReadData(HDC1080_Data);
	HDC1080_GetResult(HDC1080_Data,&HDC1080_Temperature,&HDC1080_Humidity);
	HDC1080_ConvertResultToInteger(&HDC1080_Temperature,&HDC1080_Humidity,&HDC1080_Temperature_TX,&HDC1080_Humidity_TX);
}

void main_task__manage_data(void)
{
	printf("BH1750: %d \r\n",BH1750_Illumination_TX);
	printf("HDC1080: %d , %d \r\n",HDC1080_Temperature_TX,HDC1080_Humidity_TX);
	printf("CCS811: CO2=%d \r\n", CCS811.eco2);
	printf("Soil %d \r\n", ADC_Percent);
	
	uint8_t DataPackage[12] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,'\n','\0'};
	
	DataPackage[0] = (uint8_t)(BH1750_Illumination_TX >>8);
	DataPackage[1] = (uint8_t)(BH1750_Illumination_TX & 0xff);
	DataPackage[2] = (uint8_t)(HDC1080_Temperature_TX >>8);
	DataPackage[3] = (uint8_t)(HDC1080_Temperature_TX & 0xff);
	DataPackage[4] = (uint8_t)(HDC1080_Humidity_TX >>8);
	DataPackage[5] = (uint8_t)(HDC1080_Humidity_TX & 0xff);
	DataPackage[6] = (uint8_t)(CCS811.eco2 >>8);
	DataPackage[7] = (uint8_t)(CCS811.eco2 & 0xff);
	DataPackage[8] = (uint8_t)(ADC_Percent >>8);
	DataPackage[9] = (uint8_t)(ADC_Percent & 0xff);	
	
	sendByte_via(&DebugUART,DataPackage+0);
	sendByte_via(&DebugUART,DataPackage+1);
	sendByte_via(&DebugUART,DataPackage+2);
	sendByte_via(&DebugUART,DataPackage+3);
	sendByte_via(&DebugUART,DataPackage+4);
	sendByte_via(&DebugUART,DataPackage+5);
	sendByte_via(&DebugUART,DataPackage+6);
	sendByte_via(&DebugUART,DataPackage+7);
	sendByte_via(&DebugUART,DataPackage+8);
	sendByte_via(&DebugUART,DataPackage+9);
	
	DEBUG_LED_OFF();
}





void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
}


void Error_Handler(void)
{

}

