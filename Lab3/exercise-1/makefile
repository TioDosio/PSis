CC = gcc
CFLAGS = -Wall
LIBS = -lncurses

all: human-control-client server

human-control-client: human-control-client.c remote-char.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

server: server.c remote-char.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f human-control-client server

