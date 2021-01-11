#include "main.h"

int main() {

  RCC_Init();
  GPIO_Init();

  while(1) 
  {
    GPIO_TogglePin(LED_Green_Port, LED_Green_Pin);
    Delay(500);
  }
}