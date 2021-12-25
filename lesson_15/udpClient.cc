#include "udpClient.hpp"

void Usage(std::string proc){
	std::cout << "Usage:" << proc << "server_ip, server_port" << std::endl;
}

int main(int argc, char *argv[]){
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}

	udpClient uc(argv[1], atoi(argv[2]));
	uc.InitClient();
	uc.Start();
	return 0;
}
