

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "systick.h"

#include "I2C_gpio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(void)
{
	float g_tmp,g_frh;
	
	LED_TEST_Config();
	USART_Config();
	SYSTICK_Config();
	
	SENSOR_1_I2C_Config();
	SENSOR_1_I2C_Reset();
	
	while (1)
	{

	unsigned short buff[6];
	unsigned short tem=0,rh=0;
	
	SENSOR_1_I2C_Start();
	SENSOR_1_I2C_SendByte(0x88);
	SENSOR_1_I2C_WaitAck();
	SENSOR_1_I2C_SendByte(0x2C);
	SENSOR_1_I2C_WaitAck();
	SENSOR_1_I2C_SendByte(0x06);
	SENSOR_1_I2C_WaitAck();
	
	SENSOR_1_I2C_Start();
	
	SENSOR_1_I2C_SendByte(0x89);
	SENSOR_1_I2C_WaitAck();
	
	buff[0]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	buff[1]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	buff[2]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	buff[3]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	buff[4]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	buff[5]=SENSOR_1_I2C_ReadByte();
	SENSOR_1_I2C_Ack();
	
	SENSOR_1_I2C_Stop();
	
	tem=((buff[0]<<8)|buff[1]);
	rh=((buff[3]<<8)|buff[4]);
	
	g_tmp=175.0*(float)tem/65536.0-45.0;
	g_frh=100.0*(float)rh/65536.0;
	
	printf(" %f , %f \r\n",g_tmp,g_frh);
	
	Delay_ms(1000);

	}
}

