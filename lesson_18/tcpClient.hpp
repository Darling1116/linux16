#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <signal.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct tcpClient{
	private:
		std::string svr_ip;
		int svr_port;
		int sock;
	public:
		tcpClient(std::string _ip, int _port):svr_ip(_ip), svr_port(_port){
		}

		void initClient(){
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(sock < 0){
				std::cout << "socket error!\n" << std::endl; 
				exit(2);
			}
			
			struct sockaddr_in svr;
			svr.sin_family = AF_INET;
			svr.sin_port = htons(svr_port);
			svr.sin_addr.s_addr = inet_addr(svr_ip.c_str());   //ʹip���char��
			if(connect(sock, (struct sockaddr*)&svr, sizeof(svr)) != 0){
				std::cout << "connect error!\n" << std::endl; 
				exit(3);
			}
		}


		void start(){
			char msg[1024];
			//while(true){  
				//������ֻ����һ����Ϣ���˳��ˣ����ǿͻ��˻��ڴ�����read���ݣ����԰�ѭ����ȥ��
				std::cout << "Please Enter Message:\n" << std::endl;
				fflush(stdout);
				//������˶�ȡ��Ϣ��msg
				ssize_t s = read(0, msg, sizeof(msg)-1);
				if(s > 0){
					msg[s-1] = 0;
					send(sock, msg, sizeof(msg)-1, 0);

					size_t ss = recv(sock, msg, sizeof(msg)-1, 0);  
					if(ss > 0){
						msg[ss] = 0;
						std::cout << "server echo# " << msg << std::endl;
					}
				}
			//}
		}


		~tcpClient(){
			close(sock);
		}
};

#endif