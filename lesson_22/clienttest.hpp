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
		}

		~client(){
			close(sock);
		}
};
