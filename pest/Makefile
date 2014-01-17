CXX=clang++
CXXFLAGS=-Iinclude -I/usr/include/boost
LDFLAGS=-L/usr/lib

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp, obj/%.o, $(SRC))
DEP=$(patsubst src/%.cpp, dep/%.d, $(SRC))

.PHONY: all clean

all: pest

print:
	@echo ${OBJ}

pest: dep $(DEP) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@

dep/%.d: src/%.cpp dep
	$(SHELL) -ec '$(CXX) $(CXXFLAGS) -M $< | sed "s|\($*.o\) \?:|obj/\1 $@: obj |g" > $@'
	@echo '	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -c -o obj/$*.o' >> $@

include $(DEP)

obj:
	mkdir obj

dep:
	mkdir dep

clean:
	rm -rf pest obj dep
