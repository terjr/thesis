include config.mk

CFLAGS+=-static -mcpu=cortex-a9 -O0 -gdwarf-2 -mno-thumb-interwork

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, bin/%, $(SOURCES))
FOLDERS=$(shell dirname $(TARGETS))
TESTS=$(patsubst %.c, test-%, $(SOURCES))

#{'simpoint_interval': 10000000, 'work_end_checkpoint_count': None, 'l2_assoc': 8, 'l1i_assoc': 2, 'abs_max_tick': 18446744073709551615L, 'max_checkpoints': 5, 'l1i_size': '32kB', 'checkpoint_restore': None, 'l3_assoc': 16, 'checkpoint_dir': None, 'l1d_size': '64kB', 'l2_size': '2MB', 'take_checkpoints': None, 'simpoint_profile': None, 'mem_size': '512MB', 'num_cpus': 1, 'smt': False, 'rel_max_tick': None, 'bench': None, 'cpu_clock': '1700MHz', 'checker': None, 'options': '', 'num_dirs': 1, 'checkpoint_at_end': None, 'work_end_exit_count': None, 'maxinsts': None, 'fastmem': None, 'work_begin_cpu_id_exit': None, 'work_cpus_checkpoint_count': None, 'simpoint': False, 'restore_with_cpu': 'atomic', 'fast_forward': None, 'work_begin_exit_count': None, 'l2cache': True, 'at_instruction': False, 'work_begin_checkpoint_count': None, 'maxtime': None, 'ruby': None, 'standard_switch': None, 'input': '', 'l3_size': '16MB', 'num_l3caches': 0, 'init_param': 0, 'cmd': 'bin/pi/pi', 'l1d_assoc': 2, 'sys_voltage': '1.0V', 'warmup_insts': None, 'errout': '', 'work_item_id': None, 'mem_channels': 1, 'cpu_type': 'exynos_4412p', 'mem_type': 'LPDDR2_S4_1066_x32', 'repeat_switch': None, 'num_l2caches': 1, 'output': '', 'cacheline_size': 64, 'caches': True, 'sys_clock': '400MHz', 'prog_interval': None}

GEM5_OPTIONS=--debug-flags=Cache,MemoryAccess,Exec,-ExecSymbol --remote-gdb-port=0 --debug-file=trace.out
#GEM5_OPTIONS=--remote-gdb-port=0
SIM_OPTIONS=--cpu-type=exynos_4412p --mem-type=LPDDR2_S4_800_x32 --sys-clock=440MHz --cpu-clock=1700MHz --num-l3caches=0 --caches --l2cache --l2_assoc=16 --l2_size=1MB --l1d_size=32kB --l1i_size=32kB --mem-size=2048MB --l1d_assoc=4 --l1i_assoc=4

all: $(TARGETS)

.PHONY: clean test

$(TARGETS): $(FOLDERS) bin ${SOURCES}
	${CC} $(patsubst bin/%, %.c, $@) ${CFLAGS} -o $@

bin/dhrystone/dhry_2:
	echo "Done"

bin/dhrystone/dhry_1:
	echo "Done"

bin/dhrystone/dhry:
	${CC} dhrystone/dhry_1.c dhrystone/dhry_2.c  dhrystone/dhry.h  -static -o $@  -O2 -DHZ=60 -DREG=register -DTIME -DNDEBUG -DDRUNS=100000

bin/sha2/sha2: bin/sha2/sha2
	mkdir -p bin/sha2
	cp $< $@

bin/sha2/sha2:
	${CC} sha2/main.c sha2/sha2.c ${CFLAGS} -o $@

bin/sha2/main:
	echo "Done"

clean:
	rm -rf bin

purge: clean
	rm -rf m5out

test: $(subst /,-,$(TESTS))

test-%: all m5out
	${GEM5}/build/ARM/gem5.opt ${GEM5_OPTIONS} -d m5out-stable/$* ${GEM5}/configs/example/se.py -c bin/$(subst -,/,$*) ${SIM_OPTIONS}

utest-%: all m5out
	${GEM5UNSTABLE}/build/ARM/gem5.opt ${GEM5_OPTIONS} -d m5out-unstable/$* ${GEM5UNSTABLE}/configs/example/se.py -c bin/$(subst -,/,$*) ${SIM_OPTIONS}


m5out:
	mkdir -p m5out

bin:
	mkdir -p bin

bin/%: bin
	mkdir -p $@
