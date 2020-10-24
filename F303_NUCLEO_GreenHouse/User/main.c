/*
 *	By STM32F303 NUCLEO Board
 *	
 *	PIN Define:
 *	Manual Control Console:
 *		KEY_MAN_EN: PB0
 *		KEY_LMP_ON: PB1
 *		KEY_SHT_UP: PB2
 *		KEY_SHT_DW: PB3
 *		KEY_CLR_ON: PB4
 *		KEY_HTR_ON: PB5
 *		KEY_CO2_ON: PB6
 *		KEY_FAN_ON: PB7
 *		KEY_SPR_ON: PB8
 *		KEY_ALM_ON: PB9
 *	Lamp:
 *		KEY_LMP_ON: PB10
 *	Stepper Motor:
 *		StepperMotor Orange:	PB11
 *		StepperMotor Yellow:	PB12
 *		StepperMotor Pink:		PB13
 *		StepperMotor Blue:		PB14
 *
 */

#include "stm32f30x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "peripheral_device.h"


 /* Task Handle */
static TaskHandle_t Task_Handle_AllTaskCreate 	= NULL;
static TaskHandle_t Task_Handle_ManualControl 	= NULL;
static TaskHandle_t Task_Handle_Lamp 			= NULL;
static TaskHandle_t Task_Handle_Shutter 		= NULL;
static TaskHandle_t Task_Handle_AirConditioner 	= NULL;
static TaskHandle_t Task_Handle_CO2Generator 	= NULL;
static TaskHandle_t Task_Handle_Ventilator 		= NULL;
static TaskHandle_t Task_Handle_WaterSprayer 	= NULL;
static TaskHandle_t Task_Handle_Alarm 			= NULL;
	
static EventGroupHandle_t Event_Handle = NULL;

#define KEY_MAN_EN		(0x01 << 0)
#define KEY_LMP_ON		(0x01 << 1)
#define KEY_SHT_UP		(0x01 << 2)
#define KEY_SHT_DW		(0x01 << 3)
#define KEY_CLR_ON		(0x01 << 4)
#define KEY_HTR_ON		(0x01 << 5)
#define KEY_CO2_ON		(0x01 << 6)
#define KEY_FAN_ON		(0x01 << 7)
#define KEY_SPR_ON		(0x01 << 8)
#define KEY_ALM_ON		(0x01 << 9)

 /* Task */
static void AllTaskCreate		(void);		//To create all tasks in this task
static void Task_ManualControl	(void* parameter);
static void Task_Lamp			(void* parameter);
static void Task_Shutter		(void* parameter);
static void Task_AirConditioner	(void* parameter);
static void Task_CO2Generator	(void* parameter);
static void Task_Ventilator		(void* parameter);
static void Task_WaterSprayer	(void* parameter);
static void Task_Alarm			(void* parameter);

static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );	//set NVIC first, NVIC_PriorityGroup_4 is recommended by FreeRTOS
	
	DEBUG_LED_Config();
	DEBUG_KEY_Config();
	DEBUG_USART_Config();
}

static void PeripheralDevices_Init(void)
{
	ManualControl_KEY_Config();
	Lamp_Config();
	ALARM_Config();
	ULN2003_Config();
}

int main(void)
{
	BSP_Init();
	PeripheralDevices_Init();
	
	BaseType_t xReturn = pdPASS;	/* Define an xReturn to receive status, default is pdPASS */
	
	xReturn = xTaskCreate(	(TaskFunction_t )AllTaskCreate,  
							(const char*    )"AllTaskCreate",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )1, 
							(TaskHandle_t*  )&Task_Handle_AllTaskCreate	);
          
	if(xReturn == pdPASS)
		vTaskStartScheduler();   		/* Start Scheduler */
	else
		return -1;  
}


static void AllTaskCreate(void)
{
	taskENTER_CRITICAL();
	
	Event_Handle = xEventGroupCreate(); 	// create event group
	if (Event_Handle != NULL)	
		printf("Event_Handle Created!\r\n");
	
	BaseType_t xReturn = pdPASS;
	/* To create Task_ManualControl */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_ManualControl,  
							(const char*    )"Task_ManualControl",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )3, 
							(TaskHandle_t*  )&Task_Handle_ManualControl	);
	if(xReturn == pdPASS)
		printf("Task_ManualControl Created successfully!\r\n");
	
	/* To create Task_Lamp */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Lamp,  
							(const char*    )"Task_Lamp",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Lamp	);
	if(xReturn == pdPASS)
		printf("Task_Lamp Created successfully!\r\n");

	/* To create Task_Shutter */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Shutter,  
							(const char*    )"Task_Shutter",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Shutter );
	if(xReturn == pdPASS)
		printf("Task_Shutter Created successfully!\r\n");
	
	/* To create Task_AirConditioner */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_AirConditioner,  
							(const char*    )"Task_AirConditioner",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_AirConditioner );
	if(xReturn == pdPASS)
		printf("Task_AirConditioner Created successfully!\r\n");
	
	/* To create Task_CO2Generator */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_CO2Generator,  
							(const char*    )"Task_CO2Generator",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_CO2Generator );
	if(xReturn == pdPASS)
		printf("Task_CO2Generator Created successfully!\r\n");
	
	/* To create Task_Ventilator */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Ventilator,  
							(const char*    )"Task_Ventilator",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Ventilator );
	if(xReturn == pdPASS)
		printf("Task_Ventilator Created successfully!\r\n");
	
	/* To create Task_WaterSprayer */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_WaterSprayer,  
							(const char*    )"Task_WaterSprayer",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_WaterSprayer );
	if(xReturn == pdPASS)
		printf("Task_WaterSprayer Created successfully!\r\n");
	
	/* To create Task_Alarm */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Alarm,  
							(const char*    )"Task_Alarm",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Alarm	);
	if(xReturn == pdPASS)
		printf("Task_Alarm Created!\r\n");
	
	vTaskDelete(Task_Handle_AllTaskCreate); //Delete AllTaskCreate Task
  
	taskEXIT_CRITICAL();
}






/* 
 * Function:	Task_ManualControl
 * Description: This function is for Manual Control Console.
 *				When Manual Control Key is pressed, this task will scan the keys for each device.
 *				This task should have a higher priority than other tasks for devices.
 * Parameters:	None
 * Return:		None
 */
static void Task_ManualControl(void* parameter)
{	
	BaseType_t Status_ManualControl = 0;
	
	while (1)
	{
		/* When KEY for MANUAL CONTROL is pressed down, MANUAL CONTROL MODE is working */
		if(ManualControl_Key_Scan_Continue(KEY_MAN_EN_PORT,KEY_MAN_EN_PIN) == KEY_DOWN)
		{
			if(Status_ManualControl == 0)
			{
				printf("00 Manual Control Mode Start!\r\n");
				Status_ManualControl = 1;
			}
			
			/* When KEY for LAMP ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_LMP_ON_PORT,KEY_LMP_ON_PIN) == KEY_DOWN)
			{
				printf("01 KEY for LAMP ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_LMP_ON);
			}
			
			/* When KEY for SHUTTER UP is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SHT_UP_PORT,KEY_SHT_UP_PIN) == KEY_DOWN)
			{
				printf("02 KEY for SHUTTER UP is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SHT_UP);
			}
			
			/* When KEY for SHUTTER DOWN is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SHT_DW_PORT,KEY_SHT_DW_PIN) == KEY_DOWN)
			{
				printf("03 KEY for SHUTTER DOWN is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SHT_DW);
			}			
			
			/* When KEY for COOLER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_CLR_ON_PORT,KEY_CLR_ON_PIN) == KEY_DOWN)
			{
				printf("04 KEY for COOLER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_CLR_ON);
			}				
			
			/* When KEY for HEATER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_HTR_ON_PORT,KEY_HTR_ON_PIN) == KEY_DOWN)
			{
				printf("05 KEY for HEATER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_HTR_ON);
			}	
			
			/* When KEY for CO2 GENERATOR ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_CO2_ON_PORT,KEY_CO2_ON_PIN) == KEY_DOWN)
			{
				printf("06 KEY for CO2 GENERATOR ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_CO2_ON);
			}			

			/* When KEY for FAN ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_FAN_ON_PORT,KEY_FAN_ON_PIN) == KEY_DOWN)
			{
				printf("07 KEY for FAN ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_FAN_ON);
			}				

			/* When KEY for WATER SPRAYER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SPR_ON_PORT,KEY_SPR_ON_PIN) == KEY_DOWN)
			{
				printf("08 KEY for WATER SPRAYER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SPR_ON);
			}				
			
			/* When KEY for ALARM ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_ALM_ON_PORT,KEY_ALM_ON_PIN) == KEY_DOWN)
			{
				printf("09 KEY for ALARM ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_ALM_ON);
			}
		}
		else
		{
			if(Status_ManualControl == 1)
			{
				printf("Manual Control Mode Stop!\r\n");
				Status_ManualControl = 0;
			}
		}
		vTaskDelay(MANUAL_SCAN_TIME);
	}
}

/* 
 * Function:	Task_Lamp
 * Description: This function is for switch on or switch off the lamp
 *				When LAMP ON Key is pressed, lamp will be on, otherwise be off.
 * Parameters:	None
 * Return:		None
 */
static void Task_Lamp(void* parameter)
{	
	BaseType_t LampStatus = STATUS_OFF;
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
  
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_LMP_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_LMP_ON) == (KEY_LMP_ON)) 
		{		
			if(LampStatus == STATUS_OFF)
			{
				printf("Lamp ON\r\n");
				Lamp_ON();
				LampStatus = STATUS_ON;
			}
		}
		else
		{
			if(LampStatus == STATUS_ON)
			{
				printf("Lamp OFF\r\n");
				Lamp_OFF();
				LampStatus = STATUS_OFF;
			}
		}
	}
}















static void Task_Shutter(void* parameter)
{
	BaseType_t ShutterStatus = MOTOR_INIT_POSITION;
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (100) / portTICK_PERIOD_MS;
	
	//const TickType_t xTicksToWait = portMAX_DELAY;
	
	while(1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_SHT_UP,
												pdTRUE,   
												pdTRUE,
												xTicksToWait);	//after Maximum Waiting Time, 
		
		if((EventBits_Receive & KEY_SHT_UP)==KEY_SHT_UP)
		{

				ULN2003_ToTerminal();
			
			//ULN2003_Delay_ms(8000);
			Lamp_ON();
			//vTaskDelay(4000);
				printf("ccccccccccccccccccccccccccccccccccccccccc\r\n");
			
		}
		else
		{
			printf("crrrrrrrrrrrrrrrrrrrrrrrrrrrrrrc\r\n");
			Lamp_OFF();
		}

		
		
		
	
	}
}





static void Task_AirConditioner	(void* parameter)
{
	while(1)
	{
	
	}
}
static void Task_CO2Generator	(void* parameter)
{
	while(1)
	{
	
	}
}
static void Task_Ventilator		(void* parameter)
{
	while(1)
	{
	
	}
}
static void Task_WaterSprayer	(void* parameter)
{
	while(1)
	{
	
	}
}










static void Task_Alarm(void* parameter)
{	
	BaseType_t AlarmStatus = STATUS_OFF;
	EventBits_t EventBits_Receive;
	//const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
  const TickType_t xTicksToWait = ( 100) / portTICK_PERIOD_MS;
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_ALM_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_ALM_ON) == (KEY_ALM_ON)) 
		{		
			//ULN2003_ToTerminal();
			printf("lllllllllllllllllllllllllllllllllllc\r\n");
			if(AlarmStatus == STATUS_OFF)
			{
				printf("Alarm ON\r\n");
				ALARM_LIGHT_ON();
				AlarmStatus = STATUS_ON;
			}
		}
		else
		{
			if(AlarmStatus == STATUS_ON)
			{
				printf("Alarm OFF\r\n");
				ALARM_LIGHT_OFF();
				AlarmStatus = STATUS_OFF;
			}
		}
	}
}
