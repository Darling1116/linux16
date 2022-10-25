.PHONY:all
all: httpServer httpClient

httpServer:httpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
httpClient:httpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean: 
	rm -f httpServer httpClient