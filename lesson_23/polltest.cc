#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>

using namespace std;

int main(){
	struct pollfd rfds[1];
	//rfds[0].fd = 0;  //��׼����Ϊ0
	//rfds[0].events = POLLIN;

	rfds[0].fd = 1;  //��׼���Ϊ1
	rfds[0].events = POLLOUT;
	rfds[0].revents = 0;

	char buf[1024] = {0};

	std::cout << "poll begin ...\n" << std::endl;

	while(true){
		switch(poll(rfds, 1, -1)){
			case 0:
				std::cout << "timeout..." << std::endl;
				break;
			case -1:
				std::cout << "poll error..." << std::endl;
				break;
			default:
				std::cout << "events happened!\n" << std::endl;
				//���¼�����
				if(rfds[0].fd == 1 && (rfds[0].revents & POLLOUT)){  //�ں˷��ظ��û��ģ���POLLIN������һ��
					/*
					ssize_t s = read(0, buf, sizeof(buf));
					buf[s] = 0;
					std::cout << "echo# " << buf << std::endl;
					*/
					cout << "hello world" << endl;  //�����д�¼�
				}
				break;
		}
	}

	return 0;
}