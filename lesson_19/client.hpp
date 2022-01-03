#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocol.hpp"

struct client{
	private:
		std::string srv_ip;
		int srv_port;
		int sock;

	public:
		client(std::string _ip, int _port):srv_ip(_ip), srv_port(_port){
		}


		void initclient(){
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(sock < 0){
				std::cout << "socket error!" << std::endl;
				exit(2);
			}
		}


		void start( ){
			struct sockaddr_in peer;
			peer.sin_family = AF_INET;
			peer.sin_port = htons(srv_port);
			peer.sin_addr.s_addr = inet_addr(srv_ip.c_str());
			if(connect(sock, (struct sockaddr*)&peer, sizeof(peer)) != 0){
				std::cout << "connect error!" << std::endl;
				exit(3);
			}
			/*
			char msg[1024];
			while(true){
				std::cout << "Please Enter Message: " << std::endl;
				fflush(stdout);
				ssize_t s = read(0, msg, sizeof(msg)-1);
				if(s > 0){
					msg[s-1] = 0;  //去掉最后读入的'\0'
					send(sock, msg, sizeof(msg)-1, 0);
					ssize_t ss = recv(sock, msg, sizeof(msg)-1, 0);
					if(ss > 0){
						msg[ss] = 0;
						std::cout << "server echo# " << msg << std::endl;
					}
				}
			}
			*/
			request_t rq;
			response_t rsp;
			std::cout << "data1# ";
			std::cin >> rq.x;
			std::cout << "data2# ";
			std::cin >> rq.y;
			std::cout << "op# ";
			std::cin >> rq.op;
			send(sock, &rq, sizeof(rq), 0);
			recv(sock, &rsp, sizeof(rsp), 0);
			std::cout << "code: " << rsp.code <<std::endl;
			std::cout << "result: " << rsp.result <<std::endl;
		}


		void cal(int sock){
			request_t rq;  //读取数据缓冲区
			response_t rsp;  

			ssize_t s = recv(sock, &rq, sizeof(rq), 0);
			if(s > 0){
				rsp.code = 0;
				switch(rq.op){
					case '+':
						rsp.result = rq.x + rq.y;
						break;
					case '-':
						rsp.result = rq.x - rq.y;
						break;
					case '*':
						rsp.result = rq.x * rq.y;
						break;
					case '/':
						if(rq.y != 0){
							rsp.result = rq.x / rq.y;
						}
						else{
							rsp.code = 1;
						}
						break;
					case '%':
						if(rq.y != 0){
							rsp.result = rq.x % rq.y;
						}
						else{
							rsp.code = 2;
						}
						break;
					default:
						rsp.code = 3;
						break;
				}
			send(sock, &rsp, sizeof(rsp), 0);
			//一次计算结束后，关数据通信的sock套接字
			close(sock);  
			}
		}

		~client(){
			close(sock);
		}
};

#endif