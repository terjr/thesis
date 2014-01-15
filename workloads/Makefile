include config.mk

CFLAGS+=-static -mcpu=cortex-a9

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, bin/%, $(SOURCES))
FOLDERS=$(shell dirname $(TARGETS))
TESTS=$(patsubst %.c, test-%, $(SOURCES))

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
	${GEM5}/build/ARM/gem5.opt -d m5out/$* ${GEM5}/configs/example/se.py -c bin/$(subst -,/,$*)

m5out:
	mkdir m5out

bin:
	mkdir bin

bin/%: bin
	mkdir $@
