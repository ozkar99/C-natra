CC=gcc
CFLAGS=-Wall -std=c11
SOURCES= src/utils.h src/parser.h src/server.c

all: cnatra

cnatra: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ 

clean:
	rm cnatra
