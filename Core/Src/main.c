/**
 * @file main.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief 
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "pins.h"

#include "ds3231.h"
#include "uart_ring.h"

struct
{
  char strOut[100];
  uint8_t count;
  char bufIn[100];
} bufUART;

DS3231_t DS3231;
UART_Ring_t uart1;
STM32F051_UART_t* puart1 = &uart1.uart;

uint16_t delay = 500;

void UARTComander(void)
{
  if(strcmp(bufUART.bufIn, "Set date") == 0)
  {
    DS3231.Date.Date = 18;
    DS3231.Date.Month = 1;
    DS3231.Date.Year = 21;
    DS3231.Date.Day = 1;

    DS3231_SetDate(&DS3231, &DS3231.Date);
  }
  else if (strcmp(bufUART.bufIn, "Set time") == 0)
  {
    DS3231.Time.Hours = 4;
    DS3231.Time.Minutes = 47;
    DS3231.Time.Seconds = 20;

    DS3231_SetTime(&DS3231, &DS3231.Time);
  }
  else {
    const uint16_t val = atoi(bufUART.bufIn);
    if(val != 0)
    {
      delay = val;
    }
  }
  memset(bufUART.bufIn, 0, SIZE_ARR(bufUART.bufIn));
  bufUART.count = 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int main() 
{

  RCC_Init();
  GPIO_Init();
  I2C1_Init();
  UART1_Init(&uart1.uart);
  UART_Ring_Init(&uart1);

  DS3231_SetAddress(&DS3231, 0xD0);

  while(1) 
  {
    DS3231_GetDate(&DS3231);
    DS3231_GetTime(&DS3231);
    DS3231_GetTemp(&DS3231);

    while(RING_GetCount(&uart1.ring) != 0)
    {
      bufUART.bufIn[bufUART.count] = RING_Pop(&uart1.ring);

      if(bufUART.bufIn[bufUART.count] == '\n')
      {
        bufUART.bufIn[bufUART.count] = '\0';
        UARTComander();
        continue;
      }
      bufUART.count++;
    }

    snprintf(bufUART.strOut, (size_t)SIZE_ARR(bufUART.strOut), 
      "\nDate: %02d/%02d/20%02d\nTime: %02d:%02d:%02d\nCurrent Day: %d\nTemp: %02.2f\n",
      DS3231.Date.Date, DS3231.Date.Month, DS3231.Date.Year,
      DS3231.Time.Hours, DS3231.Time.Minutes, DS3231.Time.Seconds,
      DS3231.Date.Day, DS3231.Temp);

    UART_Transmit(&uart1.uart, (uint8_t*)bufUART.strOut, strlen(bufUART.strOut), 1000);
    GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);
    Delay(delay);
  }
}

void UART_RxCallback(STM32F051_UART_t* pUART)
{
  GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
}
