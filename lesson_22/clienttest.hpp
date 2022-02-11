#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

		~client(){
			close(sock);
		}
};
