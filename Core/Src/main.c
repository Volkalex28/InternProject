#include "main.h"
#include <string.h>

uint8_t buf[] = "Hello Catherine!\n";

int main() {

  RCC_Init();
  GPIO_Init();
  UART1_Init();

  UART1_ReceiveIT(buf, 1);

  while(1) 
  {
    UART1_Transmit(buf, strlen((char*)buf), 1000);// == 0 ? GPIO_SetPin(LED_GREEN_Port, LED_GREEN_Pin) : GPIO_ResetPin(LED_GREEN_Port, LED_GREEN_Pin);
    GPIO_TogglePin(LED_BLUE_Port, LED_BLUE_Pin);
    Delay(500);
  }
}

void UART1_RxCallback(void) 
{
  GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
  UART1_ReceiveIT(buf, 1);
}