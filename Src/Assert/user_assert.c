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

/**
 * @defgroup Asserts Asserts
 * @brief Custom Asserts
 * 
 * Allows you to find gross errors when passing 
 * arguments to a function in debug mode
 */

#ifdef DEBUG

/**
 * @defgroup Assert_Private_Types Private Types
 * @ingroup Asserts
 * 
 * Data type definitions that are not externally accessible
 */
///@{
typedef struct
{
  uint32_t pc;  ///< Stores the return address of the function
  uint32_t lr;  ///< Stores a program counter
} AssertInfo_t;
///@}

/**
 * @defgroup Assert_Private_Variables Private Variables
 * @ingroup Asserts
 * 
 * Variables used to form an assert that are not accessible from outside
 */
///@{
AssertInfo_t g_assert_info; ///< A variable that stores data about the place where the assert was triggered
///@}

/**
 * @defgroup Assert_Exported_Function Exported Function
 * @ingroup Asserts
 * 
 * Assertion functions that are accessible from outside
 */
///@{

/**
 * @brief The function of saving data on the activation of the assert
 * 
 * This function saves data about the place where the assert 
 * was fired into a variable @link g_assert_info  @endlink
 * 
 * @param[in] pc Programm counter
 * @param[in] lr Function return address
 * 
 * @return None
 */
void my_assert(uint32_t *pc, uint32_t *lr) {
  // File and line deliberately left empty
  g_assert_info.pc = (uint32_t)pc;
  g_assert_info.lr = (uint32_t)lr;
  __asm("bkpt 5");
  while(1)
  {

  }
}

///@}

#endif
