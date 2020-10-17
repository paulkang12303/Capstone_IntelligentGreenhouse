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
#include "manual_controller.h"
#include "environment_device.h"
#include "uln2003_28BYJ_48.h"

 /* Task Handle */
static TaskHandle_t AllTaskCreate_Handle = NULL;
static TaskHandle_t ManualControl_Task_Handle = NULL;
static TaskHandle_t Shutter_Task_Handle = NULL;
static TaskHandle_t Alarm_Task_Handle = NULL;
	
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
static void AllTaskCreate(void);		//To create all tasks in this task
static void ManualControl_Task(void* parameter);
static void Shutter_Task(void* parameter);
static void Alarm_Task(void* parameter);

static void BSP_Init(void);

int main(void)
{
	BSP_Init();
	
	ALARM_Config();
	ManualControl_KEY_Config();
	ULN2003_Config();
	
	
	BaseType_t xReturn = pdPASS;	/* Define an xReturn to receive status, default is pdPASS */
	
	xReturn = xTaskCreate(	(TaskFunction_t )AllTaskCreate,  
							(const char*    )"AllTaskCreate",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )1, 
							(TaskHandle_t*  )&AllTaskCreate_Handle	);
          
  if(xReturn == pdPASS)
    vTaskStartScheduler();   		/* Start Scheduler */
  else
    return -1;  
}


static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );	//set NVIC first, NVIC_PriorityGroup_4 is recommended by FreeRTOS
	
	DEBUG_LED_Config();
	DEBUG_KEY_Config();
	DEBUG_USART_Config();
}


static void AllTaskCreate(void)
{
	taskENTER_CRITICAL();
	
	Event_Handle = xEventGroupCreate(); // create event group
	if (Event_Handle != NULL)	
		printf("Event_Handle Created!\r\n");
	
	BaseType_t xReturn = pdPASS;
	/* To create ManualControl Task */
	xReturn = xTaskCreate(	(TaskFunction_t )ManualControl_Task,  
							(const char*    )"ManualControlTask",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )3, 
							(TaskHandle_t*  )&ManualControl_Task_Handle	);
	if(xReturn == pdPASS)
		printf("ManualControl_Task Created!\r\n");
	
	/* To create Shutter Task */
	xReturn = xTaskCreate(	(TaskFunction_t )Shutter_Task,  
							(const char*    )"ShutterTask",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Shutter_Task_Handle );
	if(xReturn == pdPASS)
		printf("Shutter_Task Created!\r\n");
	
	/* To create Alarm Task */
	xReturn = xTaskCreate(	(TaskFunction_t )Alarm_Task,  
							(const char*    )"AlarmTask",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Alarm_Task_Handle	);
	if(xReturn == pdPASS)
		printf("Alarm_Task Created!\r\n");
	
	//	
	//other code here
	//
	
	
	vTaskDelete(AllTaskCreate_Handle); //Delete AllTaskCreate Task
  
	taskEXIT_CRITICAL();
}



static void ManualControl_Task(void* parameter)
{	
	BaseType_t ManualControlStatus = 0;
	
	while (1)
	{
		/* When KEY for MANUAL CONTROL is pressed down, MANUAL CONTROL MODE is working */
		if(ManualControl_Key_Scan_Continue(KEY_MAN_EN_PORT,KEY_MAN_EN_PIN) == KEY_DOWN)
		{
			if(ManualControlStatus == 0)
			{
				printf("00 Manual Control Mode Start!\r\n");
				ManualControlStatus = 1;
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
			if(ManualControlStatus == 1)
			{
				printf("Manual Control Mode Stop!\r\n");
				ManualControlStatus = 0;
			}
		}
		vTaskDelay(MANUAL_SCAN_TIME);
	}
}

static void Shutter_Task(void* parameter)
{
	BaseType_t ShutterStatus = MOTOR_INIT_POSITION;
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = portMAX_DELAY;
	
	while(1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_SHT_UP|KEY_SHT_DW,
												pdTRUE,   
												pdFALSE,
												xTicksToWait);	//after Maximum Waiting Time, 
		
		if((EventBits_Receive&KEY_SHT_UP)==KEY_SHT_UP)
		{
			//disable KEY_SHT_DW here
			if(ShutterStatus == MOTOR_INIT_POSITION)
			{
				if(ULN2003_ToTerminal() == 1)
				{
					ShutterStatus = MOTOT_END_POSITION;
					//enable KEY_SHT_DW here
				}
			}
		}
		if((EventBits_Receive&KEY_SHT_DW)==KEY_SHT_DW)
		{
			//disable KEY_SHT_DW here
			if(ShutterStatus == MOTOT_END_POSITION)
			{
				if(ULN2003_ToInitial() == 1)
				{
					ShutterStatus = MOTOR_INIT_POSITION;
					//enable KEY_SHT_DW here
				}
			}
		}
	
	}
}


static void Alarm_Task(void* parameter)
{	
	BaseType_t AlarmStatus = STATUS_OFF;
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
  
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_ALM_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_ALM_ON) == (KEY_ALM_ON)) 
		{		
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
