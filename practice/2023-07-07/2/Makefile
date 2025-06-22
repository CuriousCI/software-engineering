CC := g++
CFLAGS := -lm -O3 
DEBUG_FLAGS := -lm -std=c++11 -ggdb -g3 -Wall -Wextra -pedantic

MOCC_SRC := $(wildcard ../../../mocc/*.cpp) 
MOCC := $(MOCC_SRC:.cpp=.o) 

LOCAL_SRC := $(wildcard *.cpp) 
LOCAL := $(LOCAL_SRC:.cpp=.o)

VERSION := 1.0 

main: $(MOCC) $(LOCAL)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(MOCC_SRC) $(LOCAL_SRC)
	$(CC) $(DEBUG_FLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f main debug outputs.txt *.o ../../mocc/*.o **/*.o
