#include "stm32f0xx.h"

#include "pins.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#include "ds3231.h"

uint8_t bufUART[256] = "Hello Vlados! You are the best!!\n";
DS3231_t DS3231;

int main() 
{

  RCC_Init();
  GPIO_Init();
  UART1_Init();
  I2C_Init();

  DS3231_SetAdress(&DS3231, 0xD0);

  UART1_ReceiveIT(bufUART, 1);

  while(1) 
  {
    DS3231_GetDate(&DS3231);
    DS3231_GetTime(&DS3231);
    DS3231_GetTemp(&DS3231);

    sprintf((char*)bufUART, "\nDate: %02d/%02d/20%02d\nTime: %02d:%02d:%02d\nCurrent Day: %d\nTemp: %02.2f\n",
      DS3231.Date.Date, DS3231.Date.Month, DS3231.Date.Year,
      DS3231.Time.Hours, DS3231.Time.Minutes, DS3231.Time.Seconds,
      DS3231.Date.Day, DS3231.Temp);

    UART1_Transmit(bufUART, strlen((char*)bufUART), 1000);
    GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);
    Delay(500);
  }
}

void UART1_RxCallback(void) 
{
  GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
  UART1_ReceiveIT(bufUART, 1);

  if(bufUART[0] == 'd')
  {
    DS3231_Date_t date;
    date.Year = 21;
    date.Month = 1;
    date.Date = 14;
    date.Day = 3;

    DS3231_SetDate(&DS3231, &date);
  }
  else if (bufUART[0] == 't')
  {
    DS3231_Time_t time;
    time.Hours = 0;
    time.Minutes = 3;
    time.Seconds = 50;

    DS3231_SetTime(&DS3231, &time);
  }
  
}