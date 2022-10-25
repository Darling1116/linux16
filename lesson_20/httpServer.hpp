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

#include <fstream>

using namespace std;


#define BACKLOG 5
#define PAGE "index.html"


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
			//bzero(&local, sizeof(local));  //清零
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
			//accept返回的是sock文件描述符，用于数据通信!!!
			while(true){
				int sock = accept(lsock, (struct sockaddr*)&peer, &len); 
				if(sock < 0){
					std::cout << "accept error!" << std::endl;
					continue;  
					//accept失败后，一直继续尝试，直到成功
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
					close(lsock);  //孙子进程把lsock关闭掉，然后开始处理任务
					//处理http协议
					EchoHttp(sock);  
					exit(0);
				}
				close(sock);
				//waitpid(-1, nullptr, 0);
				//这里waitpid的第一个参数传入的是-1，代表任意一个进程；
				//如果大于0，代表等待某一个进程
			}
		}


		void EchoHttp(int sock){
			char buffer[1024];
			recv(sock, buffer, sizeof(buffer), 0);

			std::cout << "############ http request begin ##############" << std::endl;
			std::cout << buffer << std::endl;

			//使用ifstream一次读取文件的内容！！！
			std::ifstream in(PAGE);
			if(in.is_open()){
				in.seekg(0, std::ios::end);
				size_t len = in.tellg();
				in.seekg(0, std::ios::beg);
				char* file = new char[len];
				in.read(file, len);  //从PAGE中读内容到file文件中
				in.close();

				//std::cout << file << std::endl;
				
				//服务器给客户端发送响应
				std::string status_line = "http/1.0 307 Temporary Redirect\n"; 
				std::string response_header = "Content_Length: " + std::to_string(len);  
				response_header += "\n";
				response_header += "location: http://www.qq.com/";
				response_header += "\n";
				std::string blank = "\n";  

				send(sock, status_line.c_str(), status_line.size(), 0);  //响应行
				send(sock, response_header.c_str(), response_header.size(), 0);  //响应报头
				send(sock, blank.c_str(), blank.size(), 0);  //空格

				send(sock, file, len, 0);  //响应正文		
				delete []file;
			}
			std::cout << "############ http request end ###############" << std::endl;
			close(sock);
			exit(0);
		}
	


		~server(){
			close(lsock);
		}
};

#endif