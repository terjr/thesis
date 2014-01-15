include config.mk

CFLAGS+=-static -mcpu=cortex-a9

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, %, $(SOURCES))
TESTS=$(patsubst %.c, test-%, $(SOURCES))

all: $(TARGETS)

.PHONY: clean test

clean:
	rm -f $(TARGETS)

test: $(shell echo ${TESTS} |  sed 's|/|-|g' )

test-%: all
	${GEM5}/build/ARM/gem5.opt ${GEM5}/configs/example/se.py -c $(subst -,/,$*)


