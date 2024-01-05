all: clean client

client: 
	gcc -g Roach-client.c -o Roach-client -lzmq -lncurses -lpthread

clean:
	rm -f Roach-client