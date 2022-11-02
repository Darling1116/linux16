#include "epollClient.hpp"

void Usage(std::string proc){
	std::cout << proc << "end_point_ip,end_point_port" << std::endl;
}

int main(int argc, char* argv[]){
	if(argc != 3){
		Usage(argv[0]);  //主机的名称
		exit(1);
	}
	client *epc = new client(argv[1], atoi(argv[2]));
	epc->initclient();
	epc->start();
	return 0;
}