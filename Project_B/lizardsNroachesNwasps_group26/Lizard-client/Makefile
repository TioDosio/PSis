all: clean client 

client: 
	gcc -g Lizard-client.c ../common-files/display-funcs.c -o Lizard-client -lzmq -lncurses -lpthread

clean:
	rm -f Lizard-client