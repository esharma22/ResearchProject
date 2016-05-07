#Setting compiler
CC=g++

#Flags for compiler
CFLAGS=-c -Wall -std=c++11

all: make_meth

make_meth: noise.o regions.o
	$(CC) noise.o regions.o -o make_meth

noise.o : regions.h noise.cpp
	$(CC) $(CFLAGS) noise.cpp

regions.o: regions.h regions.cpp
	$(CC) $(CFLAGS) regions.cpp

clean:
	rm *.o make_meth
