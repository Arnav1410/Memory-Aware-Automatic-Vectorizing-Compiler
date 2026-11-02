CC ?= gcc
CFLAGS ?= -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -O2

all: tinyc1

tinyc1: lex.yy.c main.o symbol_table.o
	$(CC) $(CFLAGS) lex.yy.c main.o symbol_table.o -o $@

lex.yy.c: tinyc.l
	flex tinyc.l

main.o: main.c symbol_table.h
	$(CC) $(CFLAGS) -c main.c -o $@

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c -o $@

clean:
	rm -f tinyc1 lex.yy.c *.o

.PHONY: all clean
