# Makefile for server-exercise-3, human-client, and machine-client

CC = gcc
CFLAGS = -Wall -Wextra 
LIBS = -lncurses -lzmq

all: server-exercise-3 human-client machine-client

server-exercise-3: server-exercise-3.c
	$(CC) $(CFLAGS) -o server-exercise-3 server-exercise-3.c $(LIBS)

human-client: human-client.c
	$(CC) $(CFLAGS) -o human-client human-client.c $(LIBS)

machine-client: machine-client.c
	$(CC) $(CFLAGS) -o machine-client machine-client.c $(LIBS)

clean:
	rm -f server-exercise-3 human-client machine-client
