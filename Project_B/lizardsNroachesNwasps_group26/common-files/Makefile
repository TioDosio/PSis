all: protobuff

messages.pb-c.h: messages.proto
	protoc --c_out=. messages.proto

clean: 
	rm messages.pb-c.h