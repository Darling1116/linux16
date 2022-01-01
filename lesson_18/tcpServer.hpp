#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

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

#include "ThreadPool.hpp"

#define BACKLOG 5 

struct tcpServer{
	private:
		int port;
		int lsock;
		ThreadPool *tp;
	public:
		tcpServer(int _port):port(_port), lsock(-1), tp(nullptr){
		}

		void initServer(){
			lsock = socket(AF_INET, SOCK_STREAM, 0);
			if(lsock < 0){
				std::cout << "socket error!\n" << std::endl; 
				exit(2);
			}
			std::cout << "lsock: "  << lsock << std::endl; 

			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);
			local.sin_addr.s_addr = INADDR_ANY;
			if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
				std::cout << "bind error!\n" << std::endl; 
				exit(3);
			}

			if(listen(lsock, BACKLOG) < 0){
				std::cout << "listen error!\n" << std::endl; 
				exit(4);
			}
			
			tp = new ThreadPool(10);
			tp->ThreadPoolInit();
		}


		void start(){
			struct sockaddr_in end_point;
			socklen_t len = sizeof(end_point);
			while(true){
				//accept返回的是sock文件描述符，用于数据通信
				int sock = accept(lsock, (struct sockaddr*)&end_point, &len);  
				if(sock < 0){
					std::cout << "accept error!\n" << std::endl; 
					continue;
				}
				char buf[16];
				//利用sprintf把字符串放到buf中
				sprintf(buf, "%d", ntohs(end_point.sin_port));  
				std::string cli = inet_ntoa(end_point.sin_addr);  //inet_ntoa:网络转为字符型
				cli += ":";
				cli += buf;
				std::cout << "client info: "  << cli << " 1sock:" << sock << std::endl; 
				//开始通信服务
				//service(sock);  
				Task *t = new Task(sock);
				tp->Put(*t);
			}
		}


		void service(int sock){
			char msg[1024];
			while(true){
				ssize_t s = recv(sock, msg, sizeof(msg)-1 ,0);
				if(s > 0){
					msg[s] = 0;
					std::cout << "client# "  << msg << std::endl;
					std::string echo = msg;
					echo += " [server echo!]";
					send(sock, echo.c_str(), echo.size(), 0); 
				}
				else if(s == 0){
					std::cout << "client quit...\n" << std::endl;
					break;
				}
				else{
					std::cout << "recv msg erroe...\n" << std::endl;
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