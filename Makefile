CC=gcc
CFLAGS=-Wall -std=c11 -g
SOURCES=src/server.c src/parser.c
LIBS=src/parser.h src/utils.h

all: cnatra

cnatra: $(SOURCES) $(LIBS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ 

clean:
	rm cnatra
