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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f051_gpio.h>
#include <stm32f051_rcc.h>

#include "main.h"
#include "pins.h"
#include "uart_ring.h"

#include "FreeRTOSInit.h"

#include "esp8266.h"
#include <ds3231.h>

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
static struct
{
  char strOut[SIZE_UART_BUFFERS]; ///< The string that is sent over the UART
                                  ///< interface
  char bufIn[SIZE_UART_BUFFERS];  ///< Command input buffer
} bufUART;

static DS3231_t DS3231;    ///< Real time clock module object
static uint16_t delay = 1; ///< Stores the number of seconds of the RTC module polling pause
///@}

// Exported variable -----------------------------------------------------------
/**
 * @defgroup Main_Exported_Variable Main Exported Variable
 * @ingroup Main_program_body
 * @brief Variables used in the program loop and accessible from the outside
 */
///@{
UART_Ring_t uart1; ///< UART module object for ESP with circular buffer support
UART_Ring_t uart2; ///< UART module object for PC with circular buffer support
///@}

// Private Functions ----------------------------------------------------------
/**
 * @defgroup Main_Private_Functions Main Private Functions
 * @ingroup Main_program_body
 * @brief Functions used in the program loop and are not accessible from the
 * outside
 */
///@{

/**
 * @brief Handler of commands received via the UART protocol
 *
 * The function is called after a newline character (\\n) arrives and executes
 one of the commands:
 * @verbatim
 Set time - Sets the time in the RTC module
 Set date - Sets the date to the RTC module
 Number   - Sending a number to the controller sets the polling time of the RTC
 module in milliseconds
 @endverbatim
 *
 * @return None
 */
void UARTComander(void)
{
  if (strcmp(bufUART.bufIn, "Set date") == 0)
  {
    DS3231.Date.Date = 15;
    DS3231.Date.Month = 3;
    DS3231.Date.Year = 21;
    DS3231.Date.Day = 1;

    DS3231_SetDate(&DS3231, &DS3231.Date);
  }
  else if (strcmp(bufUART.bufIn, "Set time") == 0)
  {
    DS3231.Time.Hours = 19;
    DS3231.Time.Minutes = 33;
    DS3231.Time.Seconds = 20;

    DS3231_SetTime(&DS3231, &DS3231.Time);
  }
  else
  {
    const uint16_t val = atoi(bufUART.bufIn);
    if (val != 0)
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
 * This is where the initial initialization of the periphery and internal
 * structures of the program takes place, and the logic of the program is also
 * implemented.
 *
 * @return int
 *
 * @note Function exit never occurs
 */
int main(void)
{
  RCC_Init();
  GPIO_Init();
  UART_Ring_Init(&uart1, UART1_INIT);
  UART_Ring_Init(&uart2, UART2_INIT);

  DS3231_SetAddress(&DS3231, 0xD0);

  FreeRTOS_Init();
  vTaskStartScheduler();

  while (1)
  {
    Delay(500);
    GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
  }
  return EXIT_FAILURE;
}

/**
 * Toggles the green LED status when the expected number of bytes is received
 */
void UART_RxCallback(STM32F051_UART_t * pUART)
{
  GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
}

void ConnectWIFI(void)
{
  xSemaphoreTake(endReadUART, portMAX_DELAY);

  const uint8_t lengthStr = strlen(bufUART.bufIn);
  uint16_t positionComa;
  for (positionComa = 0; positionComa < lengthStr; positionComa++)
  {
    if (bufUART.bufIn[positionComa] == ',')
    {
      break;
    }
  }
  ESP_ConnectWIFI(bufUART.bufIn, bufUART.bufIn + positionComa, 3000);

  xSemaphoreGive(endCommandUART);
}

void taskLoop(void * pvParameters)
{
  ESP_Init(1000);
  ESP_Scan();
  vTaskDelay(3000);
  GPIO_SetPin(LED_BLUE_Port, LED_BLUE_Pin);
  ESP_PrintSSIDs(&uart2.uart, 3000);
  ConnectWIFI();
  ESP_ConnectServer("192.168.0.140", 3390, 3000);

  GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);

  while (1)
  {
    if (xSemaphoreTake(endReadUART, 0) == pdTRUE)
    {
      UARTComander();
      xSemaphoreGive(endCommandUART);
    }

    DS3231_GetDate(&DS3231);
    DS3231_GetTime(&DS3231);
    DS3231_GetTemp(&DS3231);

    snprintf(bufUART.strOut, (size_t)SIZE_ARR(bufUART.strOut),
      "\nDate: %02d/%02d/20%02d\nTime: %02d:%02d:%02d\nCurrent Day: "
      "%d\nTemp: %02d.%02d\n",
      DS3231.Date.Date, DS3231.Date.Month, DS3231.Date.Year, DS3231.Time.Hours, DS3231.Time.Minutes,
      DS3231.Time.Seconds, DS3231.Date.Day, (int)DS3231.Temp, (int)(DS3231.Temp * 100) % 100);

    ESP_SendToServer(bufUART.strOut, 3000);

    vTaskDelay(delay * 1000);
  }

  vTaskDelete(NULL);
}

void taskReadESP(void * pvParameters)
{
  uint8_t temp = 0;
  while (1)
  {
    while (UART_Ring_PopByte(&uart1, &temp) != 0)
    {
      ESP_Parse(temp);
    }
    taskYIELD();
    vTaskDelay(1);
  }

  vTaskDelete(NULL);
}

void taskReadUART(void * pvParametres)
{
  while (1)
  {
    xSemaphoreTake(endCommandUART, portMAX_DELAY);
    while (1)
    {
      int8_t pos = UART_Ring_GetStr(&uart2, bufUART.bufIn, SIZE_ARR(bufUART.bufIn), '\n');
      if (pos < 0)
      {
        taskYIELD();
        vTaskDelay(1);
        continue;
      }

      bufUART.bufIn[pos] = '\0';
      xSemaphoreGive(endReadUART);
      break;
    }

    taskYIELD();
    vTaskDelay(1);
  }

  vTaskDelete(NULL);
}
