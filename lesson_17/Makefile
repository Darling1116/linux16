.PHONY:all
all: tcpServer tcpClient test

tcpServer:tcpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcpClient:tcpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread
test:test.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpServer tcpClient