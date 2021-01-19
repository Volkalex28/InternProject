#ifndef _ASSETS_H_
#define _ASSETS_H_

#include <stdint.h>

// Convenience structure that we can store items in
//  to print out later when we add logging.
typedef struct {
  uint32_t pc;
  uint32_t lr;
  uint32_t line;
} sAssertInfo_t;

#define GET_LR() __builtin_return_address(0)

#define GET_PC(_a) __asm volatile ("mov %0, pc" : "=r" (_a))

#define ASSERT_RECORD()     \
  do {                         \
    void *pc;                  \
    GET_PC(pc);                \
    const void *lr = GET_LR(); \
    my_assert(pc, (uint32_t *)lr);         \
  } while (0)

#define ASSERT(exp)         \
  do {                         \
    if (!(exp)) {              \
      ASSERT_RECORD();      \
    }                          \
  } while (0)

extern sAssertInfo_t g_assert_info;

void my_assert(uint32_t *pc, uint32_t *lr);

#endif // _ASSETS_H_
