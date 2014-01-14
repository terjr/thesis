#include "loop_util.h"

#define TEST_BLOCK(instr1, instr2, label, iterations) \
    printf(LOG_PREFIX"Running "instr1" and "instr2" for "iterations" iterations\n"); \
    CACHE_WARMUP(label); \
    FAST_LOOP_nHAZARDS(label, instr1, instr2, iterations); \

#define TEST_BLOCK_3(instr1, instr2, instr3, label, iterations) \
    printf(LOG_PREFIX"Running "instr1", "instr2" and "instr3" for "iterations" iterations\n"); \
    CACHE_WARMUP(label); \
    FAST_LOOP_3(label, instr1, instr2, instr3, iterations); \

#define TEST_BENCH(instr1, instr2) \
    INIT_RANDOM_REGVALS(); \
\
    /* Loop for power-meassure */ \
    TEST_BLOCK(instr1, instr2, "lbl", num_iterations); \
\
    /* Loops for instruction length (cycles) determination */ \
    TEST_BLOCK(instr1, instr2, "part1", "2"); \
    TEST_BLOCK(instr1, instr2, "part2", "6"); \
    TEST_BLOCK(instr1, instr2, "part3", "0xfa"); \
    TEST_BLOCK(instr1, instr2, "part4", "0xfb");
