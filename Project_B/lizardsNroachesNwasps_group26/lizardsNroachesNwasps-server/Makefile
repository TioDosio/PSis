all: clean server

server: 
	gcc -g lizardsNroachesNwasps-server.c display-funcs.c entity_behaviour.c thread-funcs-lizard.c thread-funcs-msg.c thread-funcs-npc.c -o lizardsNroachesNwasps -lzmq -pthread -lncurses

clean:
	rm -f lizardNroachesNwasps