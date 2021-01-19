#include "user_assert.h"

sAssertInfo_t g_assert_info;

void my_assert(uint32_t *pc, uint32_t *lr) {
  // File and line deliberately left empty
  g_assert_info.pc = (uint32_t)pc;
  g_assert_info.lr = (uint32_t)lr;
  __asm("bkpt 5");
}
