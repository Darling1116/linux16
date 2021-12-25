FLAG=-std=c++11

.PHONY:all
all:udpServer udpClient

udpServer:udpServer.cc
	g++ -o $@ $^ $(FLAG)
udpClient:udpClient.cc
	g++ -o $@ $^ $(FLAG)

.PHONY:clean
clean: 
	rm -f udpServer udpClient