#include "udpServer.hpp"

/*
void Usage(std::string proc){
	std::cout << "Usage: " << proc << "local_ip, local_port" << std::endl;
}

//����main������������������ip��port��¶���������ڵ���
int main(int argc, char *argv[]){
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}
*/


//----ʵ�ֶ������칦��----
//��ָ��ip(ʹ��INADDR_ANY)
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