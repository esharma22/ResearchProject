#Setting compiler
CC=g++

#Flags for compiler
CFLAGS=-c -Wall -std=c++11

all: make_meth

make_meth: main.o regions.o
	$(CC) main.o regions.o -o make_meth

main.o : regions.h main.cpp
	$(CC) $(CFLAGS) main.cpp

regions.o: regions.h regions.cpp
	$(CC) $(CFLAGS) regions.cpp

clean:
	rm *.o make_meth
