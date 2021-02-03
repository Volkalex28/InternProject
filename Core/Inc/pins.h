/**
 * @file pins.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Custom naming for pins and their ports
 * @version 0.1
 * @date 2021-01-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PINS_H_
#define _PINS_H_

/** 
 * @defgroup User_Named_Pins User named pins
 * @ingroup Core
 * @brief Redefined pin and port names for better code usability
 */
///@{

#define LED_GREEN_Port  GPIOC
#define LED_GREEN_Pin   GPIO_PIN_9
#define LED_BLUE_Port   GPIOC
#define LED_BLUE_Pin    GPIO_PIN_8

///@}

#endif // _PINS_H_
