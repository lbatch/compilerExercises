#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS =  

OBJS = main.o util.o scan.o parse.o symtab.o analyze.o code.o cgen.o

tiny.exe: $(OBJS)
	$(CC) $(CFLAGS) -etiny $(OBJS) -o tiny

main.o: main.c globals.h util.h scan.h parse.h analyze.h cgen.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c parse.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) $(CFLAGS) -c analyze.c

code.o: code.c code.h globals.h
	$(CC) $(CFLAGS) -c code.c

cgen.o: cgen.c globals.h symtab.h code.h cgen.h
	$(CC) $(CFLAGS) -c cgen.c

clean:
	rm tiny.exe
	rm tm.exe
	rm -f main.o
	rm -f util.o
	rm -f scan.o
	rm -f parse.o
	rm -f symtab.o
	rm -f analyze.o
	rm -f code.o
	rm -f cgen.o
	rm -f tm.o

tm.exe: tm.c
	$(CC) $(CFLAGS) -etm tm.c

tiny: tiny.exe

tm: tm.exe

all: tiny tm

