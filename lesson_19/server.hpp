#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/wait.h>

#include "protocol.hpp"

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
			struct sockaddr_in end_point;
			socklen_t len = sizeof(end_point);
			//accept返回的是sock文件描述符，用于数据通信!!!
			while(true){
				int sock = accept(lsock, (struct sockaddr*)&end_point, &len); 
				if(sock < 0){
					std::cout << "accept error!" << std::endl;
					continue;  
					//accept失败后，一直继续尝试，直到成功
				}
				char buf[16];
				sprintf(buf, "%d", ntohs(end_point.sin_port));
				std::string cli = inet_ntoa(end_point.sin_addr);
				cli += ":";
				cli += buf;
				std::cout << "client info: "  << cli << " sock:" << sock << std::endl; 

				//service(sock);
				if(fork() == 0){
					if(fork() > 0){
						exit(0); 
					}
					close(lsock);  //孙子进程把lsock关闭掉，然后开始处理任务
					//计算器用短连接实现
					cal(sock);  
					exit(0);
				}
				close(sock);
				waitpid(-1, nullptr, 0);
				//这里waitpid的第一个参数传入的是-1，代表任意一个进程；
				//如果大于0，代表等待某一个进程
			}
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


		/*
		void service(int sock){
			char msg[1024];
			while(true){
				ssize_t s = recv(sock, msg, sizeof(msg)-1, 0);
				if(s > 0){
					msg[s] = 0;
					std::cout << "clinet# " << msg << std::endl;
					std::string echo = msg;
					echo += "[server echo]";
					send(sock, echo.c_str(), echo.size(), 0);
				}
				else if(s == 0){
					std::cout << "clinet quit..." << std::endl;
					close(sock);
					break;
				}
				else{
					std::cout << "recv data error..." << std::endl;
					close(sock);
					break;
				}
			}
			close(sock);
		}
		*/

		~server(){
			close(lsock);
		}
};





#endif