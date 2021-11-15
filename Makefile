# Makefile for 'indexer'
#
# Donia Tung
# CS50, Fall 2021


S = support
L = libcs50
# OBJS = server.o player.o grid.o
OBJS = client.o ../support/message.o ../support/log.o

LIBS = -lm -lncurses
LLIBS = $S/support.a $L/libcs50-given.a

# uncomment the following to turn on verbose memory logging
# TESTING= -DAPPTEST #-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$S
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test clean valgrind

all: 
	$(MAKE) -C $S
	$(MAKE) playertest
	$(MAKE) server
	make -C client

tuple: tuple.h
	$(CC) $(CFLAGS) $^ -o $@

grid: grid.o tuple.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

player: player.o grid.o tuple.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

playertest: playertest.o player.o grid.o tuple.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

server: server.o player.o grid.o tuple.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

gridtest: gridtest.o grid.o tuple.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

client: ./client/client.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


grid.o: tuple.h grid.h 
player.o: tuple.h grid.h player.h
playertest.o: player.h tuple.h grid.h 
server.o: player.h tuple.h grid.h
tuple.o: tuple.h
gridtest.o: grid.h

test:
	$(MAKE) -C $S
	$(MAKE) server
	./server maps/main.txt

valgrind:
	$(MAKE) -C $S
	$(MAKE) server
	$(VALGRIND) ./server maps/main.txt

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server gridtest player
	rm -f vgcore.*
	rm -f core
	cd $S && $(MAKE) clean
	