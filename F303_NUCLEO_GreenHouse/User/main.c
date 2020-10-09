
//using STM32F303 NUCLEO Board

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED������ */
static TaskHandle_t LED_Task_Handle = NULL;
/* UART������ */
static TaskHandle_t UART_Task_Handle = NULL;

static void AppTaskCreate(void);/* ���ڴ������� */
static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */
static void UART_Task(void* pvParameters);/* UART_Task����ʵ�� */








static void BSP_Init(void);

int main(void)
{
	BSP_Init();
	
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
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
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* ������ں��� */
                        (const char*    )"LED_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED_Task����ɹ�!\r\n");
  
  
    /* ����UART_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )UART_Task, /* ������ں��� */
                        (const char*    )"UART_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&UART_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����UART_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
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
