all: clean messages.h

messages.h: messages.proto
	protoc --c_out=. messages.proto

clean: 
	rm -f messages.h