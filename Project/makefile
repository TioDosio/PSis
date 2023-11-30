CC = gcc
CFLAGS = -Wall -Wextra 
LIBS = -lncurses -lzmq

all: Display-app.c Lizard-client.c lizardsNroaches-server.c Roaches-client.c

Display-app.c: Display-app.c
	$(CC) $(CFLAGS) Display-app Display-app.c $(LIBS)

Lizard-client.c: Lizard-client.c
	$(CC) $(CFLAGS) Lizard-client Lizard-client.c $(LIBS)

lizardsNroaches.c: lizardsNroaches.c
	$(CC) $(CFLAGS) lizardsNroaches lizardsNroaches.c $(LIBS)

Roaches-client.c: Roaches-client.c
	$(CC) $(CFLAGS) Roaches-client Roaches-client.c $(LIBS)

clean:
	rm -f Display-app Lizard-client lizardsNroaches Roaches-client
