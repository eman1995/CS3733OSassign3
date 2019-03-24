CC=gcc
CFLAGS=-I.
DEPS = part1.c

%.o: %.c %(DEPS)
	$(CC) -c -o -Wall -g $@ $< $(CFLAGS)

part1: part1.o
	$(CC) -o part1 part1.o
