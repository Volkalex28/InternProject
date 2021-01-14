
#include <string.h>
#include <stdio.h>

#include "stm32f0xx.h"

#include "main.h"
#include "pins.h"

#include "ds3231.h"

uint8_t bufUART[256] = {};
DS3231_t DS3231;

char commandFromUART = 0;

void UARTComander(void)
{
  if(commandFromUART == 'd')
  {
    DS3231_Date_t date;
    date.Date = 14;
    date.Month = 1;
    date.Year = 21;
    date.Day = 4;

    DS3231_SetDate(&DS3231, &date);
  }
  else if (commandFromUART == 't')
  {
    DS3231_Time_t time;
    time.Hours = 14;
    time.Minutes = 9;
    time.Seconds = 20;

    DS3231_SetTime(&DS3231, &time);
  }
}

int main() 
{

  RCC_Init();
  GPIO_Init();
  UART1_Init();
  I2C_Init();

  DS3231_SetAddress(&DS3231, 0xD0);

  UART1_ReceiveIT((uint8_t*)&commandFromUART, 1);

  while(1) 
  {
    if(commandFromUART != 0)
    {
      UARTComander();
      GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
      UART1_ReceiveIT((uint8_t*)&commandFromUART, 1);
      commandFromUART = 0;
    }

    DS3231_GetDate(&DS3231);
    DS3231_GetTime(&DS3231);
    DS3231_GetTemp(&DS3231);

    snprintf((char*)bufUART, (size_t)SIZE_ARR(bufUART), "\nDate: %02d/%02d/20%02d\nTime: %02d:%02d:%02d\nCurrent Day: %d\nTemp: %02.2f\n",
      DS3231.Date.Date, DS3231.Date.Month, DS3231.Date.Year,
      DS3231.Time.Hours, DS3231.Time.Minutes, DS3231.Time.Seconds,
      DS3231.Date.Day, DS3231.Temp);

    UART1_Transmit(bufUART, strlen((char*)bufUART), 1000);
    GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);
    Delay(500);
  }
}
