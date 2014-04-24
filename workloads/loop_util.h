#define num_iterations "0x1fffffff"

#define LOG_PREFIX "tdt4900:"

#define INIT_ITER_COUNT(iter_count) \
    __asm__ volatile ( \
            "mov r7, #"iter_count"\n" \
            )

#define INIT_RANDOM_REGVALS() \
    __asm__ volatile ( \
            "mov r0, #0x55\n" \
            "mov r1, #0x71\n" \
            "mov r2, #0x11\n" \
            "mov r3, #0x01\n" \
            )

#define LOOP_HEAD(label) \
    __asm__ volatile ( \
            ".balign 64\n" \
            "nop\n" \
            label":" \
            )

#define LOOP_TAIL(label) \
    __asm__ volatile ( \
            "subs r7, r7, #1\n" \
            "bne "label"\n" \
            )

#define NOP() __asm__ volatile ("nop\n")

#define NOP_x2() NOP(); NOP()
#define NOP_x4() NOP_x2(); NOP_x2()
#define NOP_x8() NOP_x4(); NOP_x4()
#define NOP_x16() NOP_x8(); NOP_x8()
#define NOP_x32() NOP_x16(); NOP_x16()

#define NOP_x59() \
    NOP_x32(); NOP_x16(); NOP_x8(); NOP_x2(); NOP()

#define CACHE_WARMUP(label) \
    INIT_ITER_COUNT("0xff"); \
    __asm__ volatile ("mov r7, r7, lsl #8\n"); \
    __asm__ volatile ( \
            "warmup"label":\n" \
            "ldr r0, "label"\n" \
            "ldr r1, "label"+32\n" \
            "subs r7, r7, #1\n" \
            "bne warmup"label"\n" \
            "mov %[instruction], r1\n" \
            : [instruction] "=r"(content));


#define FAST_LOOP_nHAZARDS(label, instr1, instr2, iter_count) \
    INIT_ITER_COUNT(iter_count); \
    LOOP_HEAD(label); \
    __asm__ volatile ( \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
        instr2"\n" \
        instr1"\n" \
       ); \
    LOOP_TAIL(label); \

#define FAST_LOOP(label, instr, iter_count) \
    INIT_ITER_COUNT(iter_count); \
    LOOP_HEAD(label); \
    __asm__ volatile ( \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        instr"\n" \
        ); \
    LOOP_TAIL(label); \

#define FAST_LOOP_3(label, instr1, instr2, instr3, iter_count) \
    INIT_ITER_COUNT(iter_count); \
    LOOP_HEAD(label); \
    __asm__ volatile ( \
        instr1"\n" \
        instr2"\n" \
        instr3"\n" \
        instr1"\n" \
        instr2"\n" \
        instr3"\n" \
        instr1"\n" \
        instr2"\n" \
        instr3"\n" \
        instr1"\n" \
        instr2"\n" \
        instr3"\n" \
        instr1"\n" \
        ); \
    LOOP_TAIL(label); \

