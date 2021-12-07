CC := g++
SRC := $(wildcard ./src/*.cpp *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))
CFLAGS := -pthread -g -std=c++11
SHELL := /bin/bash

part1.out: clean 1 $(OBJ)
	@echo Building $@
	@$(CC) $(CFLAGS) -o $@ $(OBJ)

part2.out: clean 2 $(OBJ)
	@echo Building $@
	@$(CC) $(CFLAGS) -o $@ $(OBJ)

part3.out: clean 3 $(OBJ)
	@echo Building $@
	@$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	@echo Building $@
	@$(CC) $(CFLAGS) -c -o $@ $<

%:
	@sed -i "/#define PART/c\#define PART $@" ./src/config.h
	@if [ $@ == 3 ]; \
	then \
	    sed -i "/#define PROTECT_SHARED_RESOURCE/c\#define PROTECT_SHARED_RESOURCE SPINLOCK" ./src/config.h; \
	else \
	    sed -i "/#define PROTECT_SHARED_RESOURCE/c\#define PROTECT_SHARED_RESOURCE MUTEX" ./src/config.h; \
	fi


clean:
	@rm -f *.o ./src/*.o
