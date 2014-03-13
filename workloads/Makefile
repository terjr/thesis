include config.mk

CFLAGS+=-static -mcpu=cortex-a9 -O0 -gdwarf-2 -mno-thumb-interwork

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, bin/%, $(SOURCES))
FOLDERS=$(shell dirname $(TARGETS))
TESTS=$(patsubst %.c, test-%, $(SOURCES))

GEM5_OPTIONS=--debug-flags=Cache,MemoryAccess,Exec --remote-gdb-port=0 --debug-file=trace.out
SIM_OPTIONS=--cpu-type=exynos_4412p --caches --l2cache --mem-type=LPDDR2_S4_1066_x32 --sys-clock=400MHz --cpu-clock=1.7GHz

all: $(TARGETS)

.PHONY: clean test

$(TARGETS): $(FOLDERS) bin ${SOURCES}
	${CC} $(patsubst bin/%, %.c, $@) ${CFLAGS} -o $@

bin/scrypt/scrypt:
	echo "Done"

bin/sha2-1.0.1/sha2:
	echo "Done"

bin/sha2-1.0.1/sha2speed:
	echo "Done"

bin/sha2-1.0.1/sha2prog:
	echo "Done"

bin/sha2-1.0.1/main:
	echo "Done"


clean:
	rm -rf bin

purge: clean
	rm -rf m5out

test: $(subst /,-,$(TESTS))

test-%: all m5out
	${GEM5}/build/ARM/gem5.opt ${GEM5_OPTIONS} -d /backup/m5out/$* ${GEM5}/configs/example/se.py -c bin/$(subst -,/,$*) ${SIM_OPTIONS}

m5out:
	mkdir -p m5out

bin:
	mkdir -p bin

bin/%: bin
	mkdir -p $@
