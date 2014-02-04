CXX=clang++
CXXFLAGS=-Iinclude -Ilib -I/usr/include/boost -Wall -Wextra -Weffc++ -std=c++11 -O0 -DBOOST_ALL_DYN_LINK -g
LDFLAGS=-lboost_system -lboost_program_options -lboost_thread

TEST_LDFLAGS=$(LDFLAGS) -lboost_unit_test_framework

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp, obj/%.o, $(SRC))
DEP=$(patsubst src/%.cpp, dep/%.d, $(SRC))

.PHONY: all clean test

TARGET=pest

all: test $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $@

dep/%.d: src/%.cpp dep
	$(SHELL) -ec '$(CXX) $(CXXFLAGS) -MM $< | sed "s|$*.o|$@|g" > $@'
	$(SHELL) -ec '$(CXX) $(CXXFLAGS) -MM $< | sed "s|\($*.o\) \?:|obj/\1: \$$(filter-out \$$(wildcard obj), obj) |g" >> $@'
	@echo '	$(CXX) $(CXXFLAGS) $< -c -o obj/$*.o' >> $@

-include $(DEP)

obj:
	mkdir obj

dep:
	mkdir dep

clean:
	rm -rf $(TARGET) obj dep test_pest

##### TESTS #####
TEST_SRC=$(wildcard tests/*.cpp)
TEST_OBJ=$(patsubst tests/*.cpp, obj/%.o, $(TEST_SRC)) $(OBJ:obj/main.o= )

test_pest: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) $(TEST_LDFLAGS) -o $@

test: test_pest
	./$<

