CXX=clang++
CXXFLAGS=-Iinclude -I/usr/include/boost -Wall -Wextra -Weffc++ -std=c++11
LDFLAGS=-L/usr/lib

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp, obj/%.o, $(SRC))
DEP=$(patsubst src/%.cpp, dep/%.d, $(SRC))

.PHONY: all clean

TARGET=pest

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@

dep/%.d: src/%.cpp dep
	$(SHELL) -ec '$(CXX) $(CXXFLAGS) -MM $< | sed "s|$*.o|$@|g" > $@'
	$(SHELL) -ec '$(CXX) $(CXXFLAGS) -MM $< | sed "s|\($*.o\) \?:|obj/\1: obj|g" >> $@'
	@echo '	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -c -o obj/$*.o' >> $@

-include $(DEP)

obj:
	mkdir obj

dep:
	mkdir dep

clean:
	rm -rf $(TARGET) obj dep
