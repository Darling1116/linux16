# ifndef __TCP_SERVER_H__
# define __TCP_SERVER_H__

#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

#define BACKLOG 5

struct tcpServer{
	private:
		int port;
		int lsock;  //�����׽���

	public:
		tcpServer(int _port):port(_port), lsock(-1){

		}

		void initServer(){  //��ʼ��Server������---��---�������������׽���
			lsock = socket(AF_INET, SOCK_STREAM, 0);
			if(lsock < 0){
				std::cout << "socket error!\n" << std::endl;
				exit(2);
			}
			else{
				std::cout << "lsock: " << lsock << std::endl;
			}
			
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);
			local.sin_addr.s_addr = INADDR_ANY;

			if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
				std::cout << "bind error!\n" << std::endl;
				exit(3);
			}

			if(listen(lsock, BACKLOG) < 0){  //BACKLOGΪ�ײ�ȫ���ӵ�����
				std::cout << "listen error!\n" << std::endl;
				exit(4);
			}
		}

		void service(int sock){  //���ܿͻ��˷��͵����󣬲�����Ӧ����Ϣ
			char msg[1024]; 

			while(true){
				ssize_t s = recv(sock, msg, sizeof(msg)-1, 0);
				if(s > 0){  //������Ϣ�ɹ�����ʾ�յ�����Ϣ��������Ӧ��
					msg[s] = 0;
					std::cout << "client# " << msg << std::endl;
					std::string echo = msg;
					echo += " [server echo!] ";
					send(sock, echo.c_str(), echo.size(), 0);  
				}
				else if(s == 0){
					std::cout << "client quit..." << std::endl;
					close(sock);  //�ͻ���ֹͣ������Ϣ����׽��ֹرյ�
					break;
				}
				else{
					std::cout << "recv client data error.." << std::endl;
					break;
				}
			}
			close(sock);
		}

		void start(){  
			struct sockaddr_in end_point;
			socklen_t len = sizeof(end_point);
			//tcpServerһ����ʼ��������һֱ�ȴ��Ž��ܿͻ��˷��ͽ������ӵ�����
			while(true){
				int sock = accept(lsock, (struct sockaddr*)&end_point, &len);
				if(sock < 0){  //lsock����ȡ������		sock:����������
					std::cout << "accept error!\n" << std::endl;
					continue;
				}
				std::cout << "get a new link...\n" << std::endl;
				service(sock);
			}
		}

		~tcpServer(){
			close(lsock);
		}
};


#endif