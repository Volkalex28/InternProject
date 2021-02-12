/**
 * @file user_assert.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Asserts implementation source file for the program
 * @version 0.1
 * @date 2021-01-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "user_assert.h"

// Private Types --------------------------------------------------------------
/**
 * @defgroup Assert_Private_Types Private Types
 * @ingroup Asserts
 * 
 * @brief Data type definitions that are not externally accessible
 */
///@{

/**
 * @brief The structure that stores data about the place where Assert was called
 */
typedef struct
{
  uint32_t pc;  ///< Stores a program counter
  uint32_t lr;  ///< Stores the return address of the function
} AssertInfo_t;
///@}

#ifdef DEBUG

// Private Variables ----------------------------------------------------------
/**
 * @defgroup Assert_Private_Variables Private Variables
 * @ingroup Asserts
 * 
 * @brief Variables used to form an assert that are not accessible from outside
 * 
 */
///@{
AssertInfo_t g_assert_info; ///< A variable that stores data about the place where the assert was triggered
///@}

// Exported Function ----------------------------------------------------------
void my_assert(uint32_t * pc, uint32_t * lr) 
{
  // File and line deliberately left empty
  g_assert_info.pc = (uint32_t)pc;
  g_assert_info.lr = (uint32_t)lr;
  __asm("bkpt 5");
  while(1)
  {

  }
}

#endif
