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
		int lsock;  //监听套接字

	public:
		tcpServer(int _port):port(_port), lsock(-1){

		}

		void initServer(){  //初始化Server：创建---绑定---监听服务器端套接字
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

			if(listen(lsock, BACKLOG) < 0){  //BACKLOG为底层全连接的数量
				std::cout << "listen error!\n" << std::endl;
				exit(4);
			}
		}

		void service(int sock){  //接受客户端发送的请求，并返回应答信息
			char msg[1024]; 

			while(true){
				ssize_t s = recv(sock, msg, sizeof(msg)-1, 0);
				if(s > 0){  //接受信息成功，显示收到的信息，并发送应答
					msg[s] = 0;
					std::cout << "client# " << msg << std::endl;
					std::string echo = msg;
					echo += " [server echo!] ";
					send(sock, echo.c_str(), echo.size(), 0);  
				}
				else if(s == 0){
					std::cout << "client quit..." << std::endl;
					close(sock);  //客户端停止发送信息后把套接字关闭掉
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
			//tcpServer一旦开始工作，就一直等待着接受客户端发送建立连接的请求
			while(true){
				int sock = accept(lsock, (struct sockaddr*)&end_point, &len);
				if(sock < 0){  //lsock：获取新链接		sock:服务新链接
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