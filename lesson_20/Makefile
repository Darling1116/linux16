.PHONY:all
all: httpServer 6httpClient

httpServer:httpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
httpClient:httpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean: 
	rm -f httpServer httpClient