all: clean server

server:
	gcc -g lizardsNroachesNwasps-server.c ../common-files/messages.pb-c.c ../common-files/display-funcs.c entity_behaviour.c thread-funcs-lizard.c thread-funcs-msg.c thread-funcs-npc.c thread-respawn.c thread-display.c thread-timeout.c -g -o lizardsNroachesNwasps -lzmq -pthread -lncurses -fcommon -lprotobuf-c

clean:
	rm -f lizardNroachesNwasps