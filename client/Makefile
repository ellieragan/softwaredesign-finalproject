#Makefile for client
#Ellie Boyd
#CS 50

#L = ../../tse-ellieragan/libcs50
S = ../support
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S #-I$L -I$S

OBJS = client.o ../support/message.o ../support/log.o
LIBS = -lcurses -lm#$C/common.a

.PHONY: all clean

all: client

client: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


test:
	bash -v testing.sh >& testing.out

clean:
	rm -f client
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f core
	rm -f vgcore.*
	rm -f *.log
