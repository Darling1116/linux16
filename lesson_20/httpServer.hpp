#ifndef __HTTPSERVER_HPP__
#define __HTTPSERVER_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BACKLOG 5

struct server{
	private:
		int port;
		int lsock;

	public:
		server(int _port):port(_port), lsock(-1){
		}


		void initServer(){

			lsock = socket(AF_INET, SOCK_STREAM, 0);
			if(lsock < 0){
				std::cout << "socket error!" << std::endl;
				exit(2);
			}
			std::cout << "lsock:" << lsock << std::endl;

			struct sockaddr_in local;
			bzero(&local, sizeof(local));  //����
			local.sin_family = AF_INET;
			local.sin_port = htons(port);
			local.sin_addr.s_addr = INADDR_ANY;
			if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
				std::cout << "bind error!" << std::endl;
				exit(3);
			}

			if(listen(lsock, BACKLOG) < 0){
				std::cout << "listen error!" << std::endl;
				exit(4);
			}
		}


		void start(){
			struct sockaddr_in peer;
			socklen_t len = sizeof(peer);
			//accept���ص���sock�ļ�����������������ͨ��!!!
			while(true){
				int sock = accept(lsock, (struct sockaddr*)&peer, &len); 
				if(sock < 0){
					std::cout << "accept error!" << std::endl;
					continue;  
					//acceptʧ�ܺ�һֱ�������ԣ�ֱ���ɹ�
				}
				char buf[16];
				sprintf(buf, "%d", ntohs(peer.sin_port));
				std::string cli = inet_ntoa(peer.sin_addr);
				cli += ":";
				cli += buf;
				std::cout << "client info: "  << cli << " sock:" << sock << std::endl; 

				if(fork() == 0){
					if(fork() > 0){
						exit(0); 
					}
					close(lsock);  //���ӽ��̰�lsock�رյ���Ȼ��ʼ��������
					//����httpЭ��
					EchoHttp(sock);  
					exit(0);
				}
				close(sock);
				//waitpid(-1, nullptr, 0);
				//����waitpid�ĵ�һ�������������-1����������һ�����̣�
				//�������0������ȴ�ĳһ������
			}
		}

		void EchoHttp(int sock){
			char request[1024];
			while(true){
				ssize_t s = recv(sock, request, sizeof(request)-1, 0);
				/////
			}
			close(sock);
		}


		~server(){
			close(lsock);
		}
};
