GCC = gcc 
AR = ar 
CFLAGS = -Wall -g -std=c99
LDFLAGS = -L.
LDLIBS = 


all: test 


test: entity.o solve.o
	$(GCC) $(CFLAGS) $^ -o $@ $(LDLIBS) $(LDFLAGS)

entity.o: entity.c entity.h solve.h
solve.o: solve.c solve.h entity.h


.PHONY: clean
clean:
	rm -f *.o test
