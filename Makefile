# Makefile for 'indexer'
#
# Donia Tung
# CS50, Fall 2021

PROG = grid
OBJS = grid.o
LIBS = ./libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb 
VFLAGS = --leak-check=full --show-leak-kinds=all
CC = gcc
MAKE = MAKE

all: grid

$(PROG): $(OBJS) $(LLIBS)
		$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: object files depend on header files
grid.o: grid.h

.PHONY: all clean test valgrind

test: #${PROG2} # ${PROG} 
	bash testing.sh &> testing.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(PROG)

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