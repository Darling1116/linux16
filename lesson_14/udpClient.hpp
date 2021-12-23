#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>

//socket�׽���ͷ�ļ�
#include <sys/types.h>
#include <sys/socket.h>

//������ַ�������ַת����ͷ�ļ�
#include <arpa/inet.h>
#include <netinet/in.h>

class udpClient{
	private:
		std::string ip;
		int port;
		int sock;
	
	public:
		//�����ip��port��Ӧ����Server��ip��port������
		udpClient(std::string _ip, int _port)
			:ip(_ip), port(_port){

		}


		void InitClient(){
		//�����׽���
			sock = socket(AF_INET, SOCK_DGRAM, 0);  //�����׽���:�ɹ�����3
			//tcp:SOCK_STREAM   udp:SOCK_DGRAM
			std::cout << "sock: " <<sock << std::endl;
		}


		void Start(){
		//�ͻ��ˣ���������-->����Ӧ��
			
			std::string msg;
			struct sockaddr_in peer;
			//socklen_t len = sizeof(peer);
			peer.sin_family = AF_INET;
			peer.sin_port = htons(port);  
			peer.sin_addr.s_addr = inet_addr(ip.c_str()); 

			for(; ; ){
				std::cout << "Please Enter# ";
				std::cin >> msg;

				if(msg == "quit"){
					break;
				}

				sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*)&peer, sizeof(peer));

				char echo[128];
				ssize_t s = recvfrom(sock, echo, sizeof(echo)-1, 0, nullptr, nullptr);
				if(s > 0){
					echo[s] = 0;
					std::cout << "server# " << echo << std::endl;
				}
			}
		}


		~udpClient(){  //��������
			close(sock);
		}

};
