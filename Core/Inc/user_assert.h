/**
 * @file user_assert.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Asserts implementation header file for the program
 * @version 0.1
 * @date 2021-01-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ASSERS_H_
#define _ASSERS_H_

#include <stdint.h>

#ifdef DEBUG

/**
 * @defgroup Assert_Exported_Macros Exported Macros
 * @ingroup Asserts
 * 
 * Assertion macros that are accessible from outside
 */
///@{

/**
 * @brief Getting the return address of a function
 */
#define GET_LR() __builtin_return_address(0)

/**
 * @brief Getting a program counter
 */
#define GET_PC(_a) __asm volatile ("mov %0, pc" : "=r" (_a))

/**
 * @brief Receiving data about the location of Assert firing and saving them
 */
#define ASSERT_RECORD()     \
  do {                         \
    void *pc;                  \
    GET_PC(pc);                \
    const void *lr = GET_LR(); \
    my_assert(pc, (uint32_t *)lr);         \
  } while (0)

/**
 * @brief Attribute Validation
 * 
 */
#define ASSERT(exp)         \
  do {                         \
    if (!(exp)) {              \
      ASSERT_RECORD();      \
    }                          \
  } while (0)
///@}

// Exported Function prototypes
void my_assert(uint32_t *pc, uint32_t *lr);

#else
  /**
   * @defgroup Assert_Exported_Macros Exported Macros
   * @ingroup Asserts
   * 
   * Plug for calling asserts
   */
  ///@{
  #define ASSERT(exp) ((void)0)
  ///@}
#endif

#endif // _ASSERS_H_
