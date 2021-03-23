/**
 * @file initFreeRTOS.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief
 *
 */

#include "FreeRTOSInit.h"
#include "main.h"

static StaticTask_t Loop_TCB;
static StackType_t Loop_Stack[configMINIMAL_STACK_SIZE];

static StaticTask_t ESP_TCB;
static StackType_t ESP_Stack[configMINIMAL_STACK_SIZE];
static StaticTask_t UART_TCB;
static StackType_t UART_Stack[configMINIMAL_STACK_SIZE];

xSemaphoreHandle endReadUART;
xSemaphoreHandle endCommandUART;

void FreeRTOS_Init(void)
{
  xTaskCreateStatic(&taskLoop, "Loop", sizeof(Loop_Stack) / sizeof(StackType_t), NULL, 1, Loop_Stack, &Loop_TCB);
  xTaskCreateStatic(&taskReadESP, "ESP", sizeof(ESP_Stack) / sizeof(StackType_t), NULL, 2, ESP_Stack, &ESP_TCB);
  xTaskCreateStatic(&taskReadUART, "UART", sizeof(UART_Stack) / sizeof(StackType_t), NULL, 2, UART_Stack, &UART_TCB);

  vSemaphoreCreateBinary(endCommandUART);
  vSemaphoreCreateBinary(endReadUART);
}

/* Idle task control block and stack */
static StaticTask_t Idle_TCB;
static StackType_t Idle_Stack[configMINIMAL_STACK_SIZE];

#if configUSE_TIMERS == 1
/* Timer task control block and stack */
static StaticTask_t Timer_TCB;
static StackType_t Timer_Stack[configTIMER_TASK_STACK_DEPTH];
#endif

/*
  vApplicationGetIdleTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
void vApplicationGetIdleTaskMemory(
  StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t * pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

#if configUSE_TIMERS == 1
/*
  vApplicationGetTimerTaskMemory gets called when
  configSUPPORT_STATIC_ALLOCATION equals to 1 and is required for static memory
  allocation support.
*/
void vApplicationGetTimerTaskMemory(
  StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer, uint32_t * pulTimerTaskStackSize)
{
  *ppxTimerTaskTCBBuffer = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
#endif
