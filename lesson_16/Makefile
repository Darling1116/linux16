FLAG=-std=c++11

.PHONY:all
all:tcpServer tcpClient

tcpServer:tcpServer.cc
	g++ -o $@ $^ $(FLAG)
tcpClient:tcpClient.cc
	g++ -o $@ $^ $(FLAG)

.PHONY:clean
clean: 
	rm  -f tcpServer tcpClient