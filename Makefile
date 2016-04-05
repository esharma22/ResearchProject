#Setting compiler
CC=g++

#Flags for compiler
CFLAGS=-c -Wall -std=c++11

all: make_meth

make_meth: make_meth.o regions.o
	$(CC) make_meth.o regions.o -o make_meth

make_meth.o : regions.h make_meth.cpp
	$(CC) $(CFLAGS) make_meth.cpp

regions.o: regions.h regions.cpp
	$(CC) $(CFLAGS) regions.cpp

clean:
	rm *.o make_meth
