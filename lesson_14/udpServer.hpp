#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//socket套接字头文件
#include <sys/types.h>
#include <sys/socket.h>

//主机地址与网络地址转换的头文件
#include <arpa/inet.h>
#include <netinet/in.h>


using namespace std;

class udpServer{
	private:
		//std::string ip;
		int port;
		int sock; //socket文件描述符
		//std::map<std::string


	public:
		//udpClient(std::string _ip="127.0.0.1", int _port=8080)
		udpServer(int _port)  //构造函数(带参)
			:port(_port){  
		}

		void InitServer(){
		//创建套接字-->绑定
			sock = socket(AF_INET, SOCK_DGRAM, 0);  //创建套接字:成功返回3
			//tcp:SOCK_STREAM   udp:SOCK_DGRAM
			std::cout << "sock: " << sock << std::endl;

			//创建sockaddr_in结构体:表示服务器端的网络协议地址
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);  //主机端口号转换为网络端口号(16位的)
			//local.sin_addr.s_addr = inet_addr(ip.c_str());  //首先转换为char类型，然后再转换为网络地址
			local.sin_addr.s_addr = INADDR_ANY;

			if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){  //绑定套接字:成功返回0，否则为-1
				std::cerr << "bind error!\n" << std::endl;
				exit(1);
			}
		}

		void Start(){
		//服务器端：接受请求-->发送应答
			char msg[64];

			for( ; ; ){
				msg[0] = '\0';
				struct sockaddr_in end_point;
				socklen_t len = sizeof(end_point);  //网络协议地址的长度，类型为socklen_t

				ssize_t s = recvfrom(sock, msg, sizeof(msg)-1, 0, 
					(struct sockaddr*)&end_point, &len);  //从远端读数据，成功返回值>0，返回类型为ssize_t

				if(s > 0){
					//读取成功，发送应答
					//返回客户端的信息(是谁发的)
					char buf[16];
					sprintf(buf, "%d", ntohs(end_point.sin_port));   //整数转字符串
					std::string cli = inet_ntoa(end_point.sin_addr);  //把4个字节字符类型的ip转为点分十进制类型的
					cli += ":";
					cli += buf;

					msg[s] = '\0';
					//std::cout << "client# " << msg << std::endl;  //输出读到的数据

					std::cout << cli << "# " << msg << std::endl;

					std::string echo_string = msg;
					echo_string += " [server echo!] ";
					sendto(sock, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr*)&end_point, len);
					
				}
				
			}

		}

		~udpServer(){  //析构函数
			close(sock);
		}
};