CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -I /usr/lib/llvm-10/include/llvm/
LDFLAGS = -std=c++17 -Wall -pedantic -O2 -L /usr/lib/llvm-10/lib/
DEBUG_FLAGS = -Wextra -D=DEBUG_ENABLE -g -O0
OBJECT_FILES = $(patsubst %.cpp, %.o, $(wildcard  *.cpp))
DEPS = $(OBJECT_FILES:.o=.d)
LIBS = -lLLVM
TARGET = bbmila

all: compile

compile: $(TARGET)

run:
	./$(TARGET)

clean:
	rm -rf $(OBJECT_FILES) $(TARGET)  $(DEPS)
deps:
	$(CXX) -MM *.cpp > Makefile.d

memcheck: debug
	valgrind --log-file=dump.core --leak-check=full ./$(TARGET)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

$(TARGET): $(OBJECT_FILES)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@ 

-include $(DEPS)

