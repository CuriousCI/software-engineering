CC := g++
CFLAGS := -lm -std=c++11 -ggdb -g3 -Wall -Wextra -pedantic

VERSION := 1.0 

%.exe: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.exe
