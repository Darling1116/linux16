# ifndef __TCPCLIENT_HPP__
# define __TCPCLIENT_HPP__

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
struct tcpClient{
	private:
		std::string svr_ip;
		int svr_port;
		int sock;  //连接套接字

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
			svr.sin_addr.s_addr = inet_addr(svr_ip.c_str());

			if(connect(sock, (struct sockaddr*)&svr, sizeof(svr)) != 0){  //不为0时，请求连接失败
				std::cout << "connect error!\n" << std::endl;
				exit(3);
			}
		}


		void start(){
			char msg[1024];
			while(true){
				std::cout << "Please Enter Message# " << std::endl;
				size_t s = read(0, msg, sizeof(msg)-1);

				if(s > 0){  //输入信息成功
					msg[s-1] = 0;
					send(sock, msg, sizeof(msg)-1, 0);  //发送信息

					size_t ss = recv(sock, msg, sizeof(msg)-1, 0);  //接受应答成功
					if(ss > 0){
						msg[ss] = 0;
						std::cout << "server echo# \n" << msg << std::endl;
					}
				}
			}
		}


		~tcpClient(){
			close(sock);
		}

};


#endif
