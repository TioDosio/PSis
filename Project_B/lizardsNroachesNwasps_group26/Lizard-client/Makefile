all: clean client 

client: 
	gcc -g Lizard-client.c ../common-files/messages.pb-c.c -o Lizard-client -lzmq -lncurses -lprotobuf-c

clean:
	rm -f Lizard-client