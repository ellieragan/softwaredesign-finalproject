#Makefile for crawler
#Ellie Boyd
#CS 50

L = ../tse-ellieragan/libcs50
S = ./support
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S

OBJS = client.o support/message.o support/log.o
LIBS = $L/libcs50.a -lcurses -lm#$C/common.a

.PHONY: all clean

all: client

client: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


test:
	bash -v testing.sh >& testing.out

#valgrind: client
	#valgrind --leak-check=full --show-leak-kinds=all ./client http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 3
clean:
	rm -f client
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f core
