
//using STM32F303 NUCLEO Board

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
/* UART任务句柄 */
static TaskHandle_t UART_Task_Handle = NULL;

static void AppTaskCreate(void);/* 用于创建任务 */
static void LED_Task(void* pvParameters);/* LED_Task任务实现 */
static void UART_Task(void* pvParameters);/* UART_Task任务实现 */








static void BSP_Init(void);

int main(void)
{
	BSP_Init();
	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
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



static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* 任务入口函数 */
                        (const char*    )"LED_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED_Task任务成功!\r\n");
  
  
    /* 创建UART_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )UART_Task, /* 任务入口函数 */
                        (const char*    )"UART_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&UART_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建UART_Task任务成功!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}

static void LED_Task(void* parameter)
{	
    while (1)
    {
        GPIO_SetBits(DEBUG_LED_PORT,DEBUG_LED_PIN);
        vTaskDelay(1000);   
 
        GPIO_ResetBits(DEBUG_LED_PORT,DEBUG_LED_PIN);    
        vTaskDelay(1000);  	 		
    }
}

static void UART_Task(void* parameter)
{	
    while (1)
    {
        printf("right\r\n");
        vTaskDelay(1000);   
 
        printf("great!\r\n");   
        vTaskDelay(1000);   	 		

    }
}
