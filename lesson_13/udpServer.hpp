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


using namespace std;

class udpServer{
	private:
		std::string ip;
		int port;
		int sock; //socket�ļ�������

	public:
		udpServer(std::string _ip="127.0.0.1", int _port=8080):ip(_ip), port(_port){  //���캯��(����)

		}


		void InitServer(){
		//�����׽���-->��
			sock = socket(AF_INET, SOCK_DGRAM, 0);  //�����׽���:�ɹ�����3
			//tcp:SOCK_STREAM   udp:SOCK_DGRAM
			std::cout << "sock: " << sock << std::endl;

			//����sockaddr_in�ṹ��:��ʾ�������˵�����Э���ַ
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_port = htons(port);  //�����˿ں�ת��Ϊ����˿ں�(16λ��)
			local.sin_addr.s_addr = inet_addr(ip.c_str());  //����ת��Ϊchar���ͣ�Ȼ����ת��Ϊ�����ַ

			if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){  //���׽���:�ɹ�����0������Ϊ-1
				std::cerr << "bind error!\n" << std::endl;
				exit(1);
			}
		}


		void Start(){
		//�������ˣ���������-->����Ӧ��
			char msg[64];

			for( ; ; ){
				msg[0] = '\0';
				struct sockaddr_in end_point;
				socklen_t  = sizeof(end_point);  //����Э���ַ�ĳ��ȣ�����Ϊsocklen_t

				ssize_t s = recvfrom(sock, msg, sizeof(msg)-1, 0, 
					(struct sockaddr*)&end_point, &len);  //��Զ�˶����ݣ��ɹ�����ֵ>0����������Ϊssize_t

				if(s > 0){
					//��ȡ�ɹ�������Ӧ��
					msg[s] = '\0';
					std::cout << "client# " << msg << std::endl;  //�������������

					std::string echo_string = msg;
					echo_string += " [server echo!] ";
					sendto(sock, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr*)&end_point, len);
					
				}
				
			}

		}


		~udpServer(){  //��������
			close(sock);
		}
};