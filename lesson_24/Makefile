.PHONY:all
all: epollServer epollClient

epollServer:epollServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread

epollClient:epollClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f epollServer epollClient