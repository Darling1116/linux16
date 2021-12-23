#include "udpServer.hpp"

/*
void Usage(std::string proc){
	std::cout << "Usage: " << proc << "local_ip, local_port" << std::endl;
}

//利用main函数的两个参数，把ip和port暴露出来，便于调整
int main(int argc, char *argv[]){
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}
*/


//----实现多人聊天功能----
//不指定ip(使用INADDR_ANY)
void Usage(std::string proc){
	std::cout << "Usage: " << proc << "local_port" << std::endl;
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