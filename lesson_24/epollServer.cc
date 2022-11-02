#include "epollServer.hpp"

void Usage(std::string proc){
	std::cout << proc << ": epoll_port"  << std::endl;
}

int main(int argc, char* argv[]){
	if(argc != 2){
		Usage(argv[0]);
		exit(1);
	}
	epollServer *eps = new epollServer(atoi(argv[1]));  //ʮ����תΪint�͵Ķ˿ں�
	eps->InitepollServer();
	eps->Start();
	delete eps;
	return 0;
}