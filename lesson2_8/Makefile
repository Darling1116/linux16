.PHONY:all
all:Server Client

Server:Server.c
	gcc -o $@ $^ 
Client:Client.c
	gcc -o $@ $^

.PHONY:clean
clean: 
	rm -f Server Client