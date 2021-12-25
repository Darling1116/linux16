#include "udpServer.hpp"

void Usage(std::string proc){
	std::cout << "Usage:" << proc << "local_port" << std::endl;
}


int main(int argc, char *argv[]){
	if(argc != 2){
		Usage(argv[0]);
		exit(1);
	}

udpServer *us = new udpServer(atoi(argv[1]));
us->InitServer();
us->Start();
delete us;
}