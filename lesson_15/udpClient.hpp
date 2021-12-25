#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

class udpClient{
	private:
		std::string ip;
		int port;
		int sock;
	
	public:
		udpClient(std::string _ip, int _port):ip(_ip), port(_port){
			
		}

		void InitClient(){
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			std::cout << "sock: " <<sock << std::endl;
		}


		void Start(){
			while(true){
				std::string msg;
				std::cout << "Client Enter# " ;
				std::cin >> msg;

				if(msg == "quit"){
					break;
				}

				struct sockaddr_in peer;
				peer.sin_family = AF_INET;
				peer.sin_port = htons(port);
				peer.sin_addr.s_addr = inet_addr(ip.c_str());

				sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*)&peer, sizeof(peer));
			
				char echo[128];
				ssize_t s = recvfrom(sock, echo, sizeof(echo)-1, 0, nullptr, nullptr);
				if(s > 0){
					echo[s] = 0;
					std::cout << "server# " << echo << std::endl;
				}
			}
		}

		~udpClient(){
			close(sock);
		}

};