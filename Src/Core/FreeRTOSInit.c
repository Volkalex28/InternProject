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

void FreeRTOS_Init(void)
{
  xTaskCreateStatic(&taskLoop, "Loop", sizeof(Loop_Stack) / sizeof(StackType_t), NULL, 1, Loop_Stack, &Loop_TCB);
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
