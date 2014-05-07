CC=gcc
CFLAGS=-Wall -std=c11 -g
SOURCES=src/server.c src/parser.c src/utils.c src/handler.c src/main.c 
INCLUDE_DIR=include/

all: cnatra

cnatra:
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $@ 

clean:
	rm cnatra
