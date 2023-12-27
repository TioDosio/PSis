CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses -lzmq

all: Display-app Lizard-client lizardsNroaches-server Roaches-client

Display-app: Display-app.c
	$(CC) $(CFLAGS) -o Display-app Display-app.c display-funcs.c $(LIBS)

Lizard-client: Lizard-client.c
	$(CC) $(CFLAGS) -o Lizard-client Lizard-client.c $(LIBS)

lizardsNroaches-server: lizardsNroaches-server.c
	$(CC) $(CFLAGS) -o lizardsNroaches-server lizardsNroaches-server.c display-funcs.c server-func.c $(LIBS)

Roaches-client: Roaches-client.c
	$(CC) $(CFLAGS) -o Roaches-client Roaches-client.c $(LIBS)

clean:
	rm -f Display-app Lizard-client lizardsNroaches-server Roaches-client
