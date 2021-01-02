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
 *		Lamp Switch: 			PB10
 *	Stepper Motor:
 *		StepperMotor Orange:	PB11
 *		StepperMotor Yellow:	PB12
 *		StepperMotor Pink:		PB13
 *		StepperMotor Blue:		PB14
 *  Air Conditioner:
 *      Heater Switch:			PC0
 *		Cooler Switch:			PC1
 *  CO2 Generator:
 *		Generator Switch:		PB15
 *  Ventilator:
 *		Ventilator Switch:		PC2
 *	Water Sprayer:
 * 		Water Sprayer Switch:	PC3
 *	Alarm:
 *		Alarm light Switch:		PC4
 *		Alarm buzzer Switch:	PC5 
 *  L432_sensors:
 *		sensor tigger:			PC6
 *	COM_UART:
 *		TX:						PC10
 *		RX:						PC11
 */

#include "stm32f30x.h"
#include "string.h"
#include "stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "peripheral_device.h"

#define		DATA_LENGTH					12

#define		ILLUMINATION_LOWERLIMIT		100
#define		ILLUMINATION_UPPERLIMIT		1000
#define		TEMPERATURE_LOWERLIMIT		2000
#define		TEMPERATURE_UPPERLIMIT		2500
#define		HUMIDITY_LOWERLIMIT			4000
#define		HUMIDITY_UPPERLIMIT			7000
#define		CO2_LOWERLIMIT				500
#define		CO2_UPPERLIMIT				800
#define		MOISTURE_LOWERLIMIT			6000
#define		MOISTURE_UPPERLIMIT			8000

 /* Task Handle */
static TaskHandle_t Task_Handle_AllTaskCreate 	= NULL;
static TaskHandle_t Task_Handle_ManualControl 	= NULL;
static TaskHandle_t Task_Handle_AutoControl 	= NULL;
static TaskHandle_t Task_Handle_DriveSensor 	= NULL;
static TaskHandle_t Task_Handle_Lamp 			= NULL;
static TaskHandle_t Task_Handle_Shutter 		= NULL;
static TaskHandle_t Task_Handle_AirConditioner 	= NULL;
static TaskHandle_t Task_Handle_CO2Generator 	= NULL;
static TaskHandle_t Task_Handle_Ventilator 		= NULL;
static TaskHandle_t Task_Handle_WaterSprayer 	= NULL;
static TaskHandle_t Task_Handle_Alarm 			= NULL;
static TaskHandle_t Task_Handle_DataHub			= NULL;
static TaskHandle_t Task_Handle_DataTransmit	= NULL;
static TaskHandle_t Task_Handle_DataShow		= NULL;

static EventGroupHandle_t Event_Handle = NULL;

xQueueHandle Queue_Handle_RxData;		//To receive data from exteral device (stm32L432)
xQueueHandle Queue_Handle_TxData;		//To send data to exteral device 
xQueueHandle Queue_Handle_LocalData;	//To manage data locally (stm32F303)
xQueueHandle Queue_Handle_ShowData;		//To show data in OLED

//xSemaphoreHandle xSemaphoreRecf;

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
static void Task_AutoControl	(void* parameter);
static void Task_DriveSensor	(void* parameter);
static void Task_Lamp			(void* parameter);
static void Task_Shutter		(void* parameter);
static void Task_AirConditioner	(void* parameter);
static void Task_CO2Generator	(void* parameter);
static void Task_Ventilator		(void* parameter);
static void Task_WaterSprayer	(void* parameter);
static void Task_Alarm			(void* parameter);
static void Task_DataHub		(void* parameter);
static void Task_DataTransmit	(void* parameter);
static void Task_DataShow		(void* parameter);

/* 
 * Function Name: 	BSP_Init
 * Description:		1. set the priority for all system
 *					2. configure the board supporting debug peripherials 
 */
static void BSP_Init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );	//set NVIC first, NVIC_PriorityGroup_4 is recommended by FreeRTOS
	
	DEBUG_LED_Config();
	DEBUG_KEY_Config();
	//DEBUG_USART_Config();
	COM_USART_Config();
}

/* 
 * Description:		1. initialize the debug peripherials 
 *					2. initialize all the peripherials and devices in this project
 *					3. create a task that will create all tasks
 *					4. start the scheduler
 */
int main(void)
{
	BSP_Init();
	PeripheralDevices_Init();
	
	welcome_info();
	
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

/* 
 * Function Name: 	AllTaskCreate
 * Description:		1. create 1 event_handle
 *					2. create 4 queues
 *					3. create all tasks
 */
static void AllTaskCreate(void)
{
	taskENTER_CRITICAL();
	
	Event_Handle = xEventGroupCreate(); 	// create event group
	if (Event_Handle != NULL)	
		printf("$_Event_Handle Created!\r\n");
	
	Queue_Handle_RxData = xQueueCreate(20, sizeof(uint8_t));	//receive data from L432 via UART2 RX pin
	if (Queue_Handle_RxData != NULL)	
		printf("$_Queue_Handle_RxData Created!\r\n");
	
	Queue_Handle_LocalData = xQueueCreate( 20, DATA_LENGTH );	//transmit data to the task for local automatic control
	if (Queue_Handle_LocalData != NULL)	
		printf("$_Queue_Handle_LocalData Created!\r\n");	
	
	Queue_Handle_ShowData = xQueueCreate( 20, DATA_LENGTH );	//transmit data to the task for showing data on OLED
	if (Queue_Handle_ShowData != NULL)	
		printf("$_Queue_Handle_ShowData Created!\r\n");	
	
	Queue_Handle_TxData = xQueueCreate( 20, DATA_LENGTH );		//transmit data to the task for sending data to wireless device
	if (Queue_Handle_TxData != NULL)	
		printf("$_Queue_Handle_TxData Created!\r\n");	
	
	BaseType_t xReturn = pdPASS;
	/* To create Task_ManualControl */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_ManualControl,  
							(const char*    )"Task_ManualControl",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )3, 
							(TaskHandle_t*  )&Task_Handle_ManualControl	);
	if(xReturn == pdPASS)
		printf("$_Task_ManualControl Created successfully!\r\n");
	
	/* To create Task_Lamp */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Lamp,  
							(const char*    )"Task_Lamp",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Lamp	);
	if(xReturn == pdPASS)
		printf("$_Task_Lamp Created successfully!\r\n");

	/* To create Task_Shutter */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Shutter,  
							(const char*    )"Task_Shutter",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Shutter );
	if(xReturn == pdPASS)
		printf("$_Task_Shutter Created successfully!\r\n");
	
	/* To create Task_AirConditioner */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_AirConditioner,  
							(const char*    )"Task_AirConditioner",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_AirConditioner );
	if(xReturn == pdPASS)
		printf("$_Task_AirConditioner Created successfully!\r\n");
	
	/* To create Task_CO2Generator */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_CO2Generator,  
							(const char*    )"Task_CO2Generator",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_CO2Generator );
	if(xReturn == pdPASS)
		printf("$_Task_CO2Generator Created successfully!\r\n");
	
	/* To create Task_Ventilator */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Ventilator,  
							(const char*    )"Task_Ventilator",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Ventilator );
	if(xReturn == pdPASS)
		printf("$_Task_Ventilator Created successfully!\r\n");
	
	/* To create Task_WaterSprayer */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_WaterSprayer,  
							(const char*    )"Task_WaterSprayer",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_WaterSprayer );
	if(xReturn == pdPASS)
		printf("$_Task_WaterSprayer Created successfully!\r\n");
	
	/* To create Task_Alarm */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_Alarm,  
							(const char*    )"Task_Alarm",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_Alarm	);
	if(xReturn == pdPASS)
		printf("$_Task_Alarm Created!\r\n");
	
	/* To create Task_DataHub */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_DataHub,  
							(const char*    )"Task_DataHub",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_DataHub	);
	if(xReturn == pdPASS)
		printf("$_Task_DataHub Created!\r\n");	
	
	/* To create Task_DataTransmit */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_DataTransmit,  
							(const char*    )"Task_DataTransmit",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_DataTransmit	);
	if(xReturn == pdPASS)
		printf("$_Task_DataTransmit Created!\r\n");	
	
	/* To create Task_DataShow */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_DataShow,  
							(const char*    )"Task_DataShow",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_DataShow	);
	if(xReturn == pdPASS)
		printf("$_Task_DataShow Created!\r\n");	
	
	/* To create Task_AutoControl */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_AutoControl,  
							(const char*    )"Task_AutoControl",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_AutoControl	);
	if(xReturn == pdPASS)
		printf("$_Task_AutoControl Created!\r\n");
	
	/* To create Task_DriveSensor */
	xReturn = xTaskCreate(	(TaskFunction_t )Task_DriveSensor,  
							(const char*    )"Task_DriveSensor",
							(uint16_t       )512, 
							(void*          )NULL,
							(UBaseType_t    )2, 
							(TaskHandle_t*  )&Task_Handle_DriveSensor	);
	if(xReturn == pdPASS)
		printf("$_Task_DriveSensor Created!\r\n");
	
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
				//printf("$_00 Manual Control Mode Start!\r\n");
				vTaskSuspend(Task_Handle_AutoControl);			//disable auto control
				Status_ManualControl = 1;
				Lamp_OFF();
				Heater_OFF();
				Cooler_OFF();
				CO2_Generator_OFF();
				Ventilator_OFF();
				Water_Sprayer_OFF();
				ALARM_LIGHT_OFF();
			}
			
			/* When KEY for LAMP ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_LMP_ON_PORT,KEY_LMP_ON_PIN) == KEY_DOWN)
			{
				//printf("$_01 KEY for LAMP ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_LMP_ON);
			}
			
			/* When KEY for SHUTTER UP is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SHT_UP_PORT,KEY_SHT_UP_PIN) == KEY_DOWN)
			{
				//printf("$_02 KEY for SHUTTER UP is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SHT_UP);
			}
			
			/* When KEY for SHUTTER DOWN is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SHT_DW_PORT,KEY_SHT_DW_PIN) == KEY_DOWN)
			{
				//printf("$_03 KEY for SHUTTER DOWN is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SHT_DW);
			}			
			
			/* When KEY for COOLER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_CLR_ON_PORT,KEY_CLR_ON_PIN) == KEY_DOWN)
			{
				//printf("$_04 KEY for COOLER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_CLR_ON);
			}				
			
			/* When KEY for HEATER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_HTR_ON_PORT,KEY_HTR_ON_PIN) == KEY_DOWN)
			{
				//printf("$_05 KEY for HEATER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_HTR_ON);
			}	
			
			/* When KEY for CO2 GENERATOR ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_CO2_ON_PORT,KEY_CO2_ON_PIN) == KEY_DOWN)
			{
				//printf("$_06 KEY for CO2 GENERATOR ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_CO2_ON);
			}			

			/* When KEY for FAN ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_FAN_ON_PORT,KEY_FAN_ON_PIN) == KEY_DOWN)
			{
				//printf("$_07 KEY for FAN ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_FAN_ON);
			}				

			/* When KEY for WATER SPRAYER ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_SPR_ON_PORT,KEY_SPR_ON_PIN) == KEY_DOWN)
			{
				//printf("$_08 KEY for WATER SPRAYER ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_SPR_ON);
			}				
			
			/* When KEY for ALARM ON is pressed down, send this event happened */
			if(ManualControl_Key_Scan_Continue(KEY_ALM_ON_PORT,KEY_ALM_ON_PIN) == KEY_DOWN)
			{
				//printf("$_09 KEY for ALARM ON is pressed down\r\n");
				xEventGroupSetBits(Event_Handle,KEY_ALM_ON);
			}
		}
		else
		{
			if(Status_ManualControl == 1)
			{
				//printf("$_10 Manual Control Mode Stop!\r\n");
				vTaskResume(Task_Handle_AutoControl);			//enable auto control
				Status_ManualControl = 0;
				
				Lamp_OFF();
				Heater_OFF();
				Cooler_OFF();
				CO2_Generator_OFF();
				Ventilator_OFF();
				Water_Sprayer_OFF();
				ALARM_LIGHT_OFF();
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
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
	
 	BaseType_t LampStatus = STATUS_OFF;
	
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
				//printf("$_Lamp ON\r\n");
				Lamp_ON();
				LampStatus = STATUS_ON;
			}
		}
		else
		{
			if(LampStatus == STATUS_ON)
			{
				//printf("$_Lamp OFF\r\n");
				Lamp_OFF();
				LampStatus = STATUS_OFF;
			}
		}
	}
}

/* 
 * Function:	Task_Shutter
 * Description: This function is for turn on or turn off the shutter
 *				
 * Parameters:	None
 * Return:		None
 */
static void Task_Shutter(void* parameter)
{
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME+100) / portTICK_PERIOD_MS;
	
	BaseType_t Status_Shutter = MOTOR_INIT_POSITION;	
	
	while(1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_SHT_UP|KEY_SHT_DW,
												pdTRUE,   
												pdFALSE,
												xTicksToWait);	
		
		if(((EventBits_Receive & KEY_SHT_UP) != 0) || ((EventBits_Receive & KEY_SHT_DW) != 0 ))
		{
			if(((EventBits_Receive & KEY_SHT_UP)==KEY_SHT_UP) && (Status_Shutter == MOTOR_INIT_POSITION))
			{
				//printf("$_Shutter is going to open\r\n");
				ULN2003_ToTerminal();
				Status_Shutter = MOTOT_END_POSITION;
				//printf("$_Shutter is opened\r\n");
			}
			if(((EventBits_Receive & KEY_SHT_DW)==KEY_SHT_DW) && (Status_Shutter == MOTOT_END_POSITION))
			{
				//printf("$_Shutter is going to close\r\n");
				ULN2003_ToInitial();
				Status_Shutter = MOTOR_INIT_POSITION;
				//printf("$_Shutter is closed\r\n");
			}
		}
	
	}
}



static void Task_AirConditioner	(void* parameter)
{
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME+100) / portTICK_PERIOD_MS;
	
	BaseType_t Status_AirConditioner = AC_CLOSE;
	
	while(1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_CLR_ON|KEY_HTR_ON,
												pdTRUE,   
												pdFALSE,
												xTicksToWait);		
		if(((EventBits_Receive & KEY_CLR_ON) != 0) || ((EventBits_Receive & KEY_HTR_ON) != 0 ))
		{
			if(((EventBits_Receive & KEY_CLR_ON)==KEY_CLR_ON) && (Status_AirConditioner == AC_CLOSE))
			{
				Status_AirConditioner = AC_COOLER;
				Cooler_ON();
				//printf("$_Cooler is on!\r\n");
			}
			if(((EventBits_Receive & KEY_HTR_ON)==KEY_HTR_ON) && (Status_AirConditioner == AC_CLOSE))
			{
				Status_AirConditioner = AC_HEATER;
				Heater_ON();
				//printf("$_Heater is on!\r\n");
			}			
		}
		else
		{
			if(Status_AirConditioner != AC_CLOSE)
			{
				Heater_OFF();
				Cooler_OFF();
				Status_AirConditioner = AC_CLOSE;
				//printf("$_Air conditioner is off\r\n");
			}
		}
	}
}

static void Task_CO2Generator	(void* parameter)
{
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
	
 	BaseType_t Status_CO2Generator = STATUS_OFF;
	
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_CO2_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_CO2_ON) == (KEY_CO2_ON)) 
		{		
			if(Status_CO2Generator == STATUS_OFF)
			{
				//printf("$_CO2 Generator ON\r\n");
				CO2_Generator_ON();
				Status_CO2Generator = STATUS_ON;
			}
		}
		else
		{
			if(Status_CO2Generator == STATUS_ON)
			{
				//printf("$_CO2 Generator OFF\r\n");
				CO2_Generator_OFF();
				Status_CO2Generator = STATUS_OFF;
			}
		}
	}
}

static void Task_Ventilator		(void* parameter)
{
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
	
 	BaseType_t Status_Ventilator = STATUS_OFF;
	
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_FAN_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_FAN_ON) == (KEY_FAN_ON)) 
		{		
			if(Status_Ventilator == STATUS_OFF)
			{
				//printf("$_Ventilator ON\r\n");
				Ventilator_ON();
				Status_Ventilator = STATUS_ON;
			}
		}
		else
		{
			if(Status_Ventilator == STATUS_ON)
			{
				//printf("$_Ventilator OFF\r\n");
				Ventilator_OFF();
				Status_Ventilator = STATUS_OFF;
			}
		}
	}
}

static void Task_WaterSprayer	(void* parameter)
{
	EventBits_t EventBits_Receive;
	const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
	
 	BaseType_t Status_WaterSprayer = STATUS_OFF;
	
	while (1)
	{
		EventBits_Receive = xEventGroupWaitBits(Event_Handle,  
												KEY_SPR_ON,
												pdTRUE,   
												pdTRUE,  
												xTicksToWait);	//after Maximum Waiting Time, 
                        
		if((EventBits_Receive & KEY_SPR_ON) == (KEY_SPR_ON)) 
		{		
			if(Status_WaterSprayer == STATUS_OFF)
			{
				//printf("$_Water Sprayer ON\r\n");
				Water_Sprayer_ON();
				Status_WaterSprayer = STATUS_ON;
			}
		}
		else
		{
			if(Status_WaterSprayer == STATUS_ON)
			{
				//printf("$_Water Sprayer OFF\r\n");
				Water_Sprayer_OFF();
				Status_WaterSprayer = STATUS_OFF;
			}
		}
	}
}

static void Task_Alarm(void* parameter)
{	
	BaseType_t AlarmStatus = STATUS_OFF;
	EventBits_t EventBits_Receive;
	//const TickType_t xTicksToWait = (MANUAL_SCAN_TIME + 100) / portTICK_PERIOD_MS; 	//Maximum Waiting Time
  const TickType_t xTicksToWait = (300) / portTICK_PERIOD_MS;
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
				//printf("$_Alarm ON\r\n");
				ALARM_LIGHT_ON();
				AlarmStatus = STATUS_ON;
			}
		}
		else
		{
			if(AlarmStatus == STATUS_ON)
			{
				//printf("$_Alarm OFF\r\n");
				ALARM_LIGHT_OFF();
				AlarmStatus = STATUS_OFF;
			}
		}
	}
}


static void Task_DriveSensor(void *parameter)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		Sensorl432_ON();
		vTaskDelayUntil(&xLastWakeTime,1000);
		Sensorl432_OFF();
		vTaskDelayUntil(&xLastWakeTime,4000);
	}
}


static void Task_DataHub(void *parameter)
{
	portCHAR Rx_Byte;
	char Rx_Data[DATA_LENGTH] = {0};
	uint8_t i = 0;
	
	while(1)
	{
		if(xQueueReceive(Queue_Handle_RxData, &Rx_Byte, 10/portTICK_RATE_MS)==pdTRUE)
		{
			Rx_Data[i++] = Rx_Byte;
			if(i >= DATA_LENGTH)
			{
				i = 0;
				xQueueSend(Queue_Handle_LocalData, 	Rx_Data,	10/portTICK_RATE_MS);
				xQueueSend(Queue_Handle_TxData, 	Rx_Data,	10/portTICK_RATE_MS);
				xQueueSend(Queue_Handle_ShowData, 	Rx_Data,	10/portTICK_RATE_MS);
			}
		}
	}
}

static void Task_DataTransmit(void *parameter)
{
	char 		Tx_DataHub[DATA_LENGTH] = {0};
	uint16_t 	Tx_Data[5] = {0};
	
	uint16_t	Illumination = 0;
	uint16_t	Temperature = 0;
	uint16_t	Humidity = 0;
	uint16_t	CO2 = 0;
	uint16_t	Moisture = 0;
	
	uint8_t StatusLAM = 0;
	uint8_t StatusSHT = 0;
	uint8_t StatusHTR = 0;
	uint8_t StatusCLR = 0;
	uint8_t StatusCO2 = 0;
	uint8_t StatusVEN = 0;
	uint8_t StatusSPR = 0;
	uint8_t StatusALM = 0;
	
	while(1)
	{
		if(xQueueReceive(Queue_Handle_TxData,Tx_DataHub,10/portTICK_RATE_MS ))
		{
			memcpy(Tx_Data, Tx_DataHub, sizeof(char)*10);
			
			char I = 1, T = 1, H =1, C = 1, S = 1;
			
			Illumination 	= Tx_Data[0];
			Temperature 	= Tx_Data[1];
			Humidity 		= Tx_Data[2];
			CO2 			= Tx_Data[3];
			Moisture 		= Tx_Data[4];
			
			if(CO2>=1600 || CO2<=300)
			{
				CO2=0;
				C = 0;
			}
			
			StatusLAM = GPIO_ReadOutputDataBit(LAMP_SWITCH_PORT,LAMP_SWITCH_PIN);
			//StatusSHT = 0;
			StatusHTR = GPIO_ReadOutputDataBit(HEATER_SWITCH_PORT,HEATER_SWITCH_PIN);
			StatusCLR = GPIO_ReadOutputDataBit(COOLER_SWITCH_PORT,COOLER_SWITCH_PIN);
			StatusCO2 = GPIO_ReadOutputDataBit(CO2_GENERATOR_SWITCH_PORT,CO2_GENERATOR_SWITCH_PIN);
			StatusVEN = GPIO_ReadOutputDataBit(VENTILATOR_SWITCH_PORT,VENTILATOR_SWITCH_PIN);
			StatusSPR = GPIO_ReadOutputDataBit(WATER_SPRAYER_SWITCH_PORT,WATER_SPRAYER_SWITCH_PIN);
			StatusALM = GPIO_ReadOutputDataBit(ALARM_LIGHT_PORT,ALARM_LIGHT_PIN);
			
			
			printf("BH1750: %.5d ",Illumination);
			printf("HDC1080: %.5d , %.5d ",Temperature,Humidity);
			printf("CCS811: CO2=%.5d ", CO2);
			printf("Soil %.5d ", Moisture);
			printf("%d%d%d%d%d ",I,T,H,C,S);
			printf("%d%d%d%d%d%d%d%d \r\n",StatusLAM,StatusSHT,StatusHTR,StatusCLR,StatusCO2,StatusVEN,StatusSPR,StatusALM);
		}
	}
}

static void Task_DataShow(void *parameter)
{
	char 		Show_DataHub[DATA_LENGTH] = {0};
	uint16_t 	Show_Data[5] = {0};
	
	uint16_t	Illumination = 0;
	uint16_t	Temperature = 0;
	uint16_t	Humidity = 0;
	uint16_t	CO2 = 0;
	uint16_t	Moisture = 0;
	
	uint8_t		temporary_data[10];

	while(1)
	{
		if(xQueueReceive(Queue_Handle_ShowData,Show_DataHub,10/portTICK_RATE_MS ))
		{
			memcpy(Show_Data, Show_DataHub, sizeof(char)*10);
			
			Illumination 	= Show_Data[0];
			Temperature 	= Show_Data[1]/100;
			Humidity 		= Show_Data[2]/100;
			CO2 			= Show_Data[3];
			if(CO2>=1600) CO2=0;
			Moisture 		= Show_Data[4]/100;

			OLED_CLS();
			OLED_ShowStr(0,1,"Illumination:",1);
			OLED_ShowStr(0,2,"Temperature:",1);
			OLED_ShowStr(0,3,"Humidity:",1);
			OLED_ShowStr(0,4,"CO2:",1);
			OLED_ShowStr(0,5,"Soil Moisture:",1);
			
			sprintf(temporary_data , "%d", Illumination);
			OLED_ShowStr(90,1,temporary_data,1);
			sprintf(temporary_data , "%d", Temperature);
			OLED_ShowStr(90,2,temporary_data,1);
			sprintf(temporary_data , "%d", Humidity);
			OLED_ShowStr(90,3,temporary_data,1);
			sprintf(temporary_data , "%d", CO2);
			OLED_ShowStr(90,4,temporary_data,1);
			sprintf(temporary_data , "%d", Moisture);
			OLED_ShowStr(90,5,temporary_data,1);
		}
	}
}


static void Task_AutoControl(void *parameter)
{
	char 		Rx_DataHub[DATA_LENGTH] = {0};
	uint16_t 	LocalData[5] = {0};
	
	uint16_t	Illumination = 0;
	uint16_t	Temperature = 0;
	uint16_t	Humidity = 0;
	uint16_t	CO2 = 0;
	uint16_t	Moisture = 0;
	
	while(1)
	{
		if(xQueueReceive(Queue_Handle_LocalData,Rx_DataHub,10/portTICK_RATE_MS ))
		{
			memcpy(LocalData, Rx_DataHub, sizeof(char)*10);
			
			Illumination 	= LocalData[0];
			Temperature 	= LocalData[1];
			Humidity 		= LocalData[2];
			CO2 			= LocalData[3];
			Moisture 		= LocalData[4];
			
			//For Illumination
			if(Illumination < ILLUMINATION_LOWERLIMIT)
			{
				Lamp_ON();
			}
			if(Illumination > ILLUMINATION_UPPERLIMIT)
			{
				Lamp_OFF();
			}
			
			//For Temperature
			if(Temperature > TEMPERATURE_UPPERLIMIT + 50)
			{
				Cooler_ON();
				Heater_OFF();
			}
			if(Temperature < TEMPERATURE_LOWERLIMIT - 50)
			{
				Heater_ON();
				Cooler_OFF();
			}
			if(Temperature <= TEMPERATURE_UPPERLIMIT && Temperature >= TEMPERATURE_LOWERLIMIT)
			{
				Cooler_OFF();
				Heater_OFF();
			}
			
			//For moisture
			if(Moisture < MOISTURE_LOWERLIMIT)
			{
				Water_Sprayer_ON();
			}
			if(Moisture >= MOISTURE_LOWERLIMIT)
			{
				Water_Sprayer_OFF();
			}
			
		}
	}
}

