include config.mk

CFLAGS+=-static -mcpu=cortex-a9

SOURCES=$(wildcard */*.c)
TARGETS=$(patsubst %.c, %, $(SOURCES))

all: $(TARGETS)

.PHONY: clean test

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	$(foreach f,$^,${GEM5}/build/ARM/gem5.opt ${GEM5}/configs/example/se.py -c $(f);)




