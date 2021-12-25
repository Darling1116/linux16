#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

class udpServer{
	private:
		int port;
		int sock;
	//---实现在线翻译系统(这里用数字代替)---//
		std::map<std::string, std::string> dict;

	public:
		udpServer(int _port):port(_port){
			dict.insert(std::pair<std::string, std::string>("haha", "apple"));
			dict.insert(std::pair<std::string, std::string>("2", "banana"));
			dict.insert(std::pair<std::string, std::string>("3", "orange"));
			dict.insert(std::pair<std::string, std::string>("4", "car"));
			dict.insert(std::pair<std::string, std::string>("5", "tree"));
			dict.insert(std::pair<std::string, std::string>("6", "flower"));
		}
		

		void InitServer(){
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			std::cout << "sock: " << sock << std::endl;

			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);
			local.sin_addr.s_addr = INADDR_ANY;
			if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
				std::cout << "bind error!\n" << std::endl;
				exit(1);
			}
		}


		void Start(){
			char msg[64];
			while(true){
				msg[0] = '\0';
				struct sockaddr_in end_point;
				socklen_t len = sizeof(end_point);
				ssize_t s = recvfrom(sock, msg, sizeof(msg)-1, 0, (struct sockaddr*)&end_point, &len);
				if(s > 0){
					char buf[16];
					sprintf(buf, "%d", ntohs(end_point.sin_port));
					std::string client = (inet_ntoa(end_point.sin_addr));
					client += ":";
					client += buf;
					std::cout << client << "# " << msg << std::endl;

					/*
					std::string echo = msg;
					echo += "[Server echo!]";
					sendto(sock, echo.c_str(), echo.size(), 0, (struct sockaddr*)&end_point, len);
					*/

					std::string echo = "unknow";
					auto it = dict.find(msg);
					if(it != dict.end()){
						echo = dict[msg];
					}
					sendto(sock, echo.c_str(), echo.size(), 0, (struct sockaddr*)&end_point, len);
				}
			}
		}

		~udpServer(){
			close(sock);
		}
		
};