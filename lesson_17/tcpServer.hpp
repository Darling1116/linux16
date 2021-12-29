#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 5 

struct tcpServer{
	private:
		int port;
		int lsock;
	public:
		tcpServer(int _port):port(_port), lsock(-1){
		}

		void initServer(){
			//�����ӽ��̷��͵�������Ϣ
			signal(SIGCHLD, SIG_IGN);

			//1.�����׽���
			lsock = socket(AF_INET, SOCK_STREAM, 0);
			if(lsock < 0){
				std::cout << "socket error!\n" << std::endl;
				exit(2);
			}
			else{
				std::cout << "lsock: " << lsock << std::endl;
			}
			//2.���׽���
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);
			local.sin_addr.s_addr = INADDR_ANY;
			if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
				std::cout << "bind error!\n" << std::endl;
				exit(3);
			}
			//3.�����׽���
			if(listen(lsock, BACKLOG) < 0){
				std::cout << "listen error!\n" << std::endl;
				exit(4);
			}
		}

		void start(){
			//4.�����׽���
			struct sockaddr_in end_point;
			socklen_t len = sizeof(end_point);
			while(true){  //һֱ�ȴ��ſͻ��˷�����������
				int sock = accept(lsock, (struct sockaddr*)&end_point, &len);
				if(sock < 0){
					std::cout << "accept error!\n" << std::endl;
					continue;
				}
				std::cout << "get a new link..." << std::endl;
				char buf[16];
				sprintf(buf, "%d", ntohs(end_point.sin_port));
				std::string cli = inet_ntoa(end_point.sin_addr);
				cli += ":";
				cli += buf;
				std::cout << "client info: " << cli << " sock: " <<sock << std::endl;

				//ʵ�ֶ���̷��������ö���ͻ��˿���ͬʱ����
				pid_t id = fork();
				if(id == 0){
					if(fork() > 0){  //�ӽ����˳�(ϵͳ�Զ���������Դ),�������ӽ���:������ֽ�ʬ����
						exit(0);
					}
					//�����ӽ��̴���IO����
					close(lsock);
					service(sock);
					exit(0);
				}
				close(sock);
			}
		}

		void service(int sock){
			char msg[1024];
			while(true){
				ssize_t s = recv(sock, msg, sizeof(msg)-1, 0);
				if(s > 0){  //�ͻ��˷�����Ϣ
					msg[s] = 0;
					std::cout << "Client# " << msg << std::endl;

					std::string echo = msg;
					echo += "[Server echo!]";
					send(sock, echo.c_str(), echo.size(), 0);
				}
				else if(s == 0){  //�ͻ���ֹͣ������Ϣ
					std::cout << "client quit..." << std::endl;
					close(sock);
					break;
				}
				else{
					std::cout << "recv client data error.." << std::endl;
					break;
				}
			}
			close(sock);
		}

		~tcpServer(){
			close(lsock);
		}
};


#endif