# Makefile for 'indexer'
#
# Donia Tung
# CS50, Fall 2021

L = libcs50
S = support
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S

OBJS = tuple.o # player.o grid.o 
LIBS = $S/support.a $L/libcs50-given.a -lcurses -lm#$C/common.a

.PHONY: all clean

all: playertest

playertest.o: tuple.o grid.o player.o player.h
playertest: playertest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

tuple.o: tuple.h

tuple: tuple.o # $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

grid.o: tuple.o grid.h

grid: grid.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

player.o: tuple.o grid.o player.h

player: player.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

test:
	bash -v testing.sh >& testing.out

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f core

# PROG = player
# OBJS = player.o grid.o
# LIBS = ./libcs50/libcs50-given.a

# CFLAGS = -Wall -pedantic -std=c11 -ggdb 
# VFLAGS = --leak-check=full --show-leak-kinds=all
# CC = gcc
# MAKE = MAKE

# all: player grid

# $(PROG): $(OBJS) $(LLIBS)
# 		$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# # Dependencies: object files depend on header files
# grid.o: grid.h
# player.o: player.h grid.h

# .PHONY: all clean test valgrind

# test: #${PROG2} # ${PROG} 
# 	bash testing.sh &> testing.out

# clean:
# 	rm -rf *.dSYM  # MacOS debugger info
# 	rm -f *~ *.o
# 	rm -f core
# 	rm -f $(PROG)