#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc
LEX = flex
YACC = bison -d

OBJS = main.o util.o lex.yy.c y.tab.c y.tab.h  symtab.o analyze.o code.o cgen.o

tiny: $(OBJS)
	$(CC) $(OBJS) -ll -lm -o tiny

main.o: main.c globals.h util.h analyze.h cgen.h
	$(CC) -c main.c

util.o: util.c util.h globals.h
	$(CC) -c util.c

symtab.o: symtab.c symtab.h
	$(CC) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) -c analyze.c

lex.yy.c: y.tab.h l.l main.c globals.h
	$(LEX) l.l

y.tab.c y.tab.h: y.y main.c globals.h
	$(YACC) y.y

code.o: code.c code.h globals.h
	$(CC) -c code.c

cgen.o: cgen.c globals.h symtab.h code.h cgen.h
	$(CC) -c cgen.c

clean:
	rm -f main.o
	rm -f util.o
	rm -f scan.o
	rm -f parse.o
	rm -f symtab.o
	rm -f analyze.o
	rm -f code.o
	rm -f cgen.o
	rm -f tm.o
	rm -f lex.yy.o
	rm -f y.tab.o

tm.exe: tm.c

tm: tm.exe

all: tiny tm

