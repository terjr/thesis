CC=../scripts/compile.sh

SOURCES=$(shell find -name '*.c')
TARGETS=$(patsubst %.c, %, $(SOURCES))

all: $(TARGETS)

.PHONY: clean test

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	$(foreach f,$^,../../gem5-stable/build/ARM/gem5.opt ../../gem5-stable/configs/example/se.py -c $(f);)




