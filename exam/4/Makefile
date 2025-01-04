CC := g++
CFLAGS := -lm -std=c++11 -ggdb -g3 -Wall -Wextra -pedantic
SRCS = $(wildcard ../case/*.cpp)
OBJS := $(SRCS:.cpp=.o)
LOCL_SRCS := $(wildcard *.cpp)
LOCL_OBJS := $(LOCL_SRCS:.cpp=.o)

VERSION := 1.0 

main: $(OBJS) $(LOCL_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o main outputs.txt ../case/*.o **/*.o **/*.mo **/*.mos **/*.dat **/run.sh **/clean.sh **/mkload.sh **/myextlib.c
