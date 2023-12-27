CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses -lzmq

all: Display-app Lizard-client lizardsNroaches-server Roaches-client

Display-app: Display-clt/Display-app.c common-files/display-funcs.c
	$(CC) $(CFLAGS) -o Display-app $^ $(LIBS)

Lizard-client: Lizard-clt/Lizard-client.c
	$(CC) $(CFLAGS) -o Lizard-client $^ $(LIBS)

lizardsNroaches-server: lizardsNroaches/lizardsNroaches-server.c lizardsNroaches/server-func.c common-files/display-funcs.c
	$(CC) $(CFLAGS) -o lizardsNroaches-server $^ $(LIBS)

Roaches-client: Roach-clt/Roaches-client.c
	$(CC) $(CFLAGS) -o Roaches-client $^ $(LIBS)

clean:
	rm -f Display-app Lizard-client lizardsNroaches-server Roaches-client
