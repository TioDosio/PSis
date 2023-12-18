#sudo apt install protobuf-c-compiler
#sudo apt install libprotobuf-c-dev
all: balls.pb-c.c server.c remote-display-client.c human-client.c


balls.pb-c.c: balls.proto
	protoc-c --c_out=. balls.proto

server.c: 
	make -C server -f Makefile all

remote-display-client.c: 
	make -C remote-display-client -f Makefile all

human-client.c:
	make -C human-client -f Makefile all

	