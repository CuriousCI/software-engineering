CC := g++
CFLAGS := -lm -O3
DEBUG_FLAGS := -lm -std=c++11 -ggdb -g3 -Wall -Wextra -pedantic

CASE_SRC := $(wildcard ../case/*.cpp) 
CASE := $(CASE_SRC:.cpp=.o) 

LOCAL_SRC := $(wildcard *.cpp) 
LOCAL := $(LOCAL_SRC:.cpp=.o)

VERSION := 1.0 

main: $(CASE) $(LOCAL)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(CASE) $(LOCAL)
	$(CC) $(GDB_FLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f main debug outputs.txt *.o ../case/*.o **/*.o *.d ../case/*.d **/*.d **/*.mo **/*.mos **/*.dat **/run.sh **/clean.sh **/mkload.sh **/myextlib.c
