#pragma once
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
		//std::string ip;
		int port;
		int sock; //socket�ļ�������
		//std::map<std::string


	public:
		//udpClient(std::string _ip="127.0.0.1", int _port=8080)
		udpServer(int _port)  //���캯��(����)
			:port(_port){  
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
			//local.sin_addr.s_addr = inet_addr(ip.c_str());  //����ת��Ϊchar���ͣ�Ȼ����ת��Ϊ�����ַ
			local.sin_addr.s_addr = INADDR_ANY;

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
				socklen_t len = sizeof(end_point);  //����Э���ַ�ĳ��ȣ�����Ϊsocklen_t

				ssize_t s = recvfrom(sock, msg, sizeof(msg)-1, 0, 
					(struct sockaddr*)&end_point, &len);  //��Զ�˶����ݣ��ɹ�����ֵ>0����������Ϊssize_t

				if(s > 0){
					//��ȡ�ɹ�������Ӧ��
					//���ؿͻ��˵���Ϣ(��˭����)
					char buf[16];
					sprintf(buf, "%d", ntohs(end_point.sin_port));   //����ת�ַ���
					std::string cli = inet_ntoa(end_point.sin_addr);  //��4���ֽ��ַ����͵�ipתΪ���ʮ�������͵�
					cli += ":";
					cli += buf;

					msg[s] = '\0';
					//std::cout << "client# " << msg << std::endl;  //�������������

					std::cout << cli << "# " << msg << std::endl;

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