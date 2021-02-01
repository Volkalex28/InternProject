/**
 * @file main.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file of main program loop
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @defgroup Core Core
 * @brief Functionality that ensures the operation of the main program cycle
 */

/**
 * @defgroup Main_program_body Main program body
 * @ingroup Core
 * @brief Contains the main program loop and program functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "pins.h"

#include "ds3231.h"
#include "uart_ring.h"

// Private macros -----------------------------------------------------------
/** 
 * @defgroup Main_Private_Macros Main Private Macros
 * @ingroup Main_program_body
 * 
 * @brief Macros used in the program loop and
 * are not accessible from the outside
 */
///@{

/**
 * @brief Size for UART input and output buffers
 */
#define SIZE_UART_BUFFERS 100
///@}

// Private variable -----------------------------------------------------------
/** 
 * @defgroup Main_Private_Variable Main Private Variable
 * @ingroup Main_program_body
 * @brief Variables used in the program loop and
 * are not accessible from the outside
 */
///@{

/**
 * @brief Responsible for the data used by the UART protocol
 */
struct
{
  char strOut[SIZE_UART_BUFFERS]; ///< The string that is sent over the UART interface
  uint8_t count;                  ///< Received bytes counter
  char bufIn[SIZE_UART_BUFFERS];  ///< Command input buffer
} bufUART;

DS3231_t DS3231;    ///< Real time clock module object
uint16_t delay = 1000; ///< Stores the number of milliseconds of the RTC module polling pause
///@}

// Exported variable -----------------------------------------------------------
/** 
 * @defgroup Main_Exported_Variable Main Exported Variable
 * @ingroup Main_program_body
 * @brief Variables used in the program loop and accessible from the outside
 */
///@{
UART_Ring_t uart1;  ///< UART module object with circular buffer support
///@}

// Private Functions ----------------------------------------------------------
/** 
 * @defgroup Main_Private_Functions Main Private Functions
 * @ingroup Main_program_body
 * @brief Functions used in the program loop and are not accessible from the outside
 */
///@{

/**
 * @brief Handler of commands received via the UART protocol
 * 
 * The function is called after a newline character (\\n) arrives and executes one of the commands:
 * @verbatim
 Set time - Sets the time in the RTC module
 Set date - Sets the date to the RTC module
 Number   - Sending a number to the controller sets the polling time of the RTC module in milliseconds
 @endverbatim
 * 
 * @return None
 */
void UARTComander(void)
{
  if(strcmp(bufUART.bufIn, "Set date") == 0)
  {
    DS3231.Date.Date = 1;
    DS3231.Date.Month = 2;
    DS3231.Date.Year = 21;
    DS3231.Date.Day = 1;

    DS3231_SetDate(&DS3231, &DS3231.Date);
  }
  else if (strcmp(bufUART.bufIn, "Set time") == 0)
  {
    DS3231.Time.Hours = 14;
    DS3231.Time.Minutes = 23;
    DS3231.Time.Seconds = 20;

    DS3231_SetTime(&DS3231, &DS3231.Time);
  }
  else 
  {
    const uint16_t val = atoi(bufUART.bufIn);
    if(val != 0)
    {
      delay = val;
    }
  }
}
///@}

/**
 * @ingroup Main_program_body
 * @brief Starting point of the program
 * 
 * This is where the initial initialization of the periphery and internal structures 
 * of the program takes place, and the logic of the program is also implemented.
 * 
 * @return int 
 * 
 * @note Function exit never occurs
 */
int main(void) 
{
  RCC_Init();
  GPIO_Init();
  I2C1_Init();
  UART_Ring_Init(&uart1);

  DS3231_SetAddress(&DS3231, 0xD0);

  while(1) 
  {
    while(1)
    {
      const uint8_t statusExtraction = UART_Ring_PopByte(&uart1, (uint8_t*)&bufUART.bufIn[bufUART.count]);

      if(statusExtraction == 1)
      {
        break;
      }
      else if(bufUART.bufIn[bufUART.count] == '\n')
      {
        UART_Transmit(&uart1.uart, (uint8_t*)bufUART.bufIn, strlen(bufUART.bufIn), 1000);

        bufUART.bufIn[bufUART.count] = '\0';
        UARTComander();
      }
      else 
      {
        bufUART.count++;

        if(bufUART.count < SIZE_ARR(bufUART.bufIn))
        {
          continue;
        }
      }
      memset(bufUART.bufIn, 0, strlen(bufUART.bufIn));
      bufUART.count = 0;
    }

    static uint32_t startTick = 0;

    if(startTick + delay < GetTick())
    {
      startTick = GetTick();

      DS3231_GetDate(&DS3231);
      DS3231_GetTime(&DS3231);
      DS3231_GetTemp(&DS3231);

      snprintf(bufUART.strOut, (size_t)SIZE_ARR(bufUART.strOut), 
        "\nDate: %02d/%02d/20%02d\nTime: %02d:%02d:%02d\nCurrent Day: %d\nTemp: %02.2f\n",
        DS3231.Date.Date, DS3231.Date.Month, DS3231.Date.Year, DS3231.Time.Hours, 
        DS3231.Time.Minutes, DS3231.Time.Seconds, DS3231.Date.Day, DS3231.Temp);

      UART_Transmit(&uart1.uart, (uint8_t*)bufUART.strOut, strlen(bufUART.strOut), 1000);
      GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);
    }

    Delay(5); 
  }
  return 1;
}

/**
 * Toggles the green LED status when the expected number of bytes is received 
 */
void UART_RxCallback(STM32F051_UART_t* pUART)
{
  GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
}
