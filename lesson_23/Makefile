.PHONY:all
all: pollServer pollClient

pollServer:pollServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread

pollClient:pollClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f pollServer pollClient