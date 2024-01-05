all: clean client

client: 
	gcc -g Roach-client.c ../common-files/messages.pb-c.c -o Roach-client -lzmq -lncurses -lpthread -lprotobuf-c

clean:
	rm -f Roach-client