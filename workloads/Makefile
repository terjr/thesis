include config.mk

CFLAGS+=-static -mcpu=cortex-a9 -O0 -gdwarf-2

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, bin/%, $(SOURCES))
FOLDERS=$(shell dirname $(TARGETS))
TESTS=$(patsubst %.c, test-%, $(SOURCES))

GEM5_OPTIONS=--debug-flags=Exec --remote-gdb-port=0 --trace-file=trace.out
SIM_OPTIONS=--cpu-type=arm_detailed --caches --l2cache

all: $(TARGETS)

.PHONY: clean test

$(TARGETS): $(FOLDERS) bin ${SOURCES}
	${CC} $(patsubst bin/%, %.c, $@) ${CFLAGS} -o $@
	

clean:
	rm -rf bin

purge: clean
	rm -rf m5out

test: $(subst /,-,$(TESTS))

test-%: all m5out
	${GEM5}/build/ARM/gem5.opt ${GEM5_OPTIONS} -d m5out/$* ${GEM5}/configs/example/se.py -c bin/$(subst -,/,$*) ${SIM_OPTIONS}

m5out:
	mkdir m5out

bin:
	mkdir bin

bin/%: bin
	mkdir $@
