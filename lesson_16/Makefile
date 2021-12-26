FLAG=-std=c++11

.PHONY:all
all:tcpServer tcpClient

tdpServer:tcpServer.cc
	g++ -o $@ $^ $(FLAG)
tdpClient:tcpClient.cc
	g++ -o $@ $^ $(FLAG)

.PHONY:clean
clean: 
	rm -f tdpServer tdpClient