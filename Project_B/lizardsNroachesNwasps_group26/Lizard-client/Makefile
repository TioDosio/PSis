all: clean client 

client: 
	gcc -g Lizard-client.c ../common-files/display-funcs.c -o Lizard-client -lzmq -lncurses -lpthread -fcommon

clean:
	rm -f Lizard-client