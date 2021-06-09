CC= gcc
CFLAGS=-std=c99 -Wall -pthread -O3
LFLAGS=-L../../lib
LIBS=-lfdsfe -lmd_message_c_wrapper
INC=-I../../include -I../../include/MD

all: simple

simple: simple.o
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/simple obj/simple.o $(LFLAGS) $(LIBS)

simple.o: src/simple.c
	mkdir -p obj
	$(CC) $(CFLAGS) -o obj/simple.o -c src/simple.c $(INC)

clean:
	rm -rf obj
	rm -rf bin
