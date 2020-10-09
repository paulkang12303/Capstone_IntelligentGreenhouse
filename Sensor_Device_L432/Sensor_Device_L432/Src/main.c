

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "I2C_gpio.h"
#include "I2C_sensors.h"

void SystemClock_Config(void);




int main(void)
{
	
	uint8_t SHT30_Data[6] = {0};
	float SHT30_Temperature = 0;
	float SHT30_Humidity = 0;
	
	uint8_t BH1750_Data[2] = {0};
	float BH1750_Illumination = 0;
	
	uint8_t HDC1080_Data[4] = {0};
	float HDC1080_Temperature = 0;
	float HDC1080_Humidity = 0;	
	
	
	HAL_Init();
	SystemClock_Config();

	LED_TEST_Config();
	DEBUG_UART_Config();

	SHT30_Config();
	
	
	if (I2C_BUS_1_CheckDevice(0x44) == 0)
	{
		LED_TEST_ON();
	}
	
	while (1)
	{
		SHT30_Start();
		SHT30_WaitingData_ms();
		SHT30_ReadData(SHT30_Data);
		SHT30_ConvertResult(SHT30_Data,&SHT30_Temperature, &SHT30_Humidity);
		printf("SHT30: %f , %f \r\n",SHT30_Temperature,SHT30_Humidity);
		
		BH1750_Start();
		BH1750_WaitingData_ms();
		BH1750_ReadData( BH1750_Data );
		BH1750_ConvertResult( BH1750_Data , &BH1750_Illumination);
		printf("BH1750: %f \r\n",BH1750_Illumination);
		
		HDC1080_Config();
		HDC1080_Start();
		HDC1080_WaitingData_ms();
		HDC1080_ReadData(HDC1080_Data);
		HDC1080_ConvertResult(HDC1080_Data,&HDC1080_Temperature, &HDC1080_Humidity);
		printf("HDC1080: %f , %f \r\n",HDC1080_Temperature,HDC1080_Humidity);
		
		
		
		HAL_Delay(1000);

	}

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
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



