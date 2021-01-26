/**
 * @file main.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file of main program loop
 * @version 0.1
 * @date 2021-01-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f0xx.h"
#include "stm32f051_rcc.h"
#include "stm32f051_gpio.h"
#include "stm32f051_uart.h"
#include "stm32f051_i2c.h"

// Exported Macros
/** 
 * @defgroup Main_Exported_Macros Main Exported Macros
 * @ingroup Main_program_body
 * @brief Macros accessible from the outside
 */
///@{
#define SIZE_ARR(_ARR_) (sizeof(_ARR_)/sizeof(_ARR_[0])) ///< Determining the size of an array
///@}

#endif /* _MAIN_H_ */
