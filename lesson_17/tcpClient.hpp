#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct tcpClient{
	private:
		std::string ip;
		int port;
		int sock;
	public:
		tcpClient(std::string _ip, int _port):ip(_ip), port(_port){
		}

		void initClient(){
			//1.创建套接字
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(sock < 0){
				std::cout << "socket error!\n" << std::endl;
				exit(2);
			}

			//2.tcp为有连接的：要给svr网络地址协议各属性赋值
			struct sockaddr_in svr;
			svr.sin_family = AF_INET;
			svr.sin_port = htons(port);
			svr.sin_addr.s_addr = inet_addr(ip.c_str());

			if(connect(sock, (struct sockaddr*)&svr, sizeof(svr)) != 0){
				std::cout << "connect error!\n" << std::endl;
				exit(3);
			}
		}


		void start(){
			char msg[1024];
			while(true){
				std::cout << "Please Enter Message# " << std::endl;
				fflush(stdout);
				ssize_t s = read(0, msg, sizeof(msg)-1); 
				if(s > 0){  //从输入端读取信息成功
					msg[s-1] = 0;  //read的时候，会把'/0'给带上
					send(sock, msg, sizeof(msg)-1, 0);

					size_t ss = recv(sock, msg, sizeof(msg)-1, 0);  
					if(ss > 0){
						msg[ss] = 0;
						std::cout << "server echo# " << msg << std::endl;
					}
				}
			}
		}


		~tcpClient(){
		}
};


#endif