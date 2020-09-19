

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "systick.h"

#include "I2C_gpio.h"
#include "I2C_sensors.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(void)
{
	LED_TEST_Config();
	USART_Config();
	SYSTICK_Config();
	
	I2C_BUS_1_Config();
	I2C_BUS_1_Reset();
	
	if (I2C_BUS_1_CheckDevice(0x44) == 0)
	{
		LED_TEST_ON();
	}
	
	while (1)
	{
	SHT30_ReadData();
	Delay_ms(1000);
	}
}


