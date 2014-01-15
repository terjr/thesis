#include "../base.c"

#undef num_iterations
#define num_iterations "0x2fffffff"


void runtest(void) {
    TEST_BENCH("addeq r1, r2, "CONST1, "addeq r3, r4, "CONST1);
}
