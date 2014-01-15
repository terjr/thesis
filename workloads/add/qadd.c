#include "../base.c"

#undef num_iterations
#define num_iterations "0x3fffffff"


void runtest(void) {
   TEST_BENCH("qadd r1, r4, r5", "qadd r2, r3, r7");
}
