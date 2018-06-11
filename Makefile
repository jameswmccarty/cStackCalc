CC = gcc
CFLAGS = -ansi -pedantic -Werror -Wall -O2
DFLAGS = -ansi -pedantic -Werror -Wall -O0 -g
LFLAGS =

all: parser stack

parser: equparser.c stack stack.h
	$(CC) $(CFLAGS) equparser.c stack.lib -o parser $(LFLAGS)

debug: equparser.c stack stack.h
	$(CC) $(DFLAGS) equparser.c stack.lib -o parser $(LFLAGS)

stack: stack.c stack.h
	$(CC) $(CFLAGS) stack.c -c -o stack.lib $(LFLAGS)

clean:
	\rm parser stack.lib
