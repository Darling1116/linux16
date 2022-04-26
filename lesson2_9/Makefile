.PHONY:all
all:Server Client test

Server:Server.c
	gcc -o $@ $^ 
Client:Client.c
	gcc -o $@ $^
test:test.c
	gcc -o $@ $^

.PHONY:clean
clean: 
	rm -f Server Client test