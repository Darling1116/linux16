#include "selectServer.hpp"

void Usage(std::string proc){
	std::cout << proc << ": server_port"  << std::endl;
}

int main(int argc, char* argv[]){
	if(argc != 2){
		Usage(argv[0]);
		exit(1);
	}
	SelectServer *s = new SelectServer(atoi(argv[1]));  //十分制转为int型的端口号
	s->InitSelectServer();
	s->Start();
	delete s;
	return 0;
}