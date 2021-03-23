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

/**
 * @defgroup Core Core
 * @brief Functionality that ensures the operation of the main program cycle
 */

/**
 * @defgroup Main_program_body Main program body
 * @ingroup Core
 * @brief Contains the main program loop and program functionality
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
  extern "C" {
#endif

// Exported Macros
/** 
 * @defgroup Main_Exported_Macros Main Exported Macros
 * @ingroup Main_program_body
 * @brief Macros accessible from the outside
 */
///@{
#define SIZE_ARR(_ARR_) (sizeof(_ARR_)/sizeof(_ARR_[0])) ///< Determining the size of an array
///@}

void taskLoop(void * pvParametres);
void taskReadESP(void * pvParameters);
void taskReadUART(void * pvParametres);

#ifdef __cplusplus
  }
#endif

#endif /* _MAIN_H_ */
