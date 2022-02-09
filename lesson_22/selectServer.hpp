#include "sock.hpp"

#include <sys/select.h>

struct SelectServer{
	private:
		int lsock; //�����׽���
		int port;
	
	public:
		SelectServer(int _port):port(_port){
		}


		void InitSelectServer(){
			lsock = Sock::Socket();  //�����׽���
			Sock::Setsockopt(lsock);  //���ö˿ں�Ϊ����״̬��
			Sock::Bind(lsock, port);  //�󶨶˿ں�
			Sock::Listen(lsock);  //�����׽���
		}

		void Start(){
			//�ڷ�����������ʱֻ��һ��listen״̬�µ��ļ�������
			int maxfd = lsock;  
			struct timeval timeout = {1000, 0};  //ÿ��5����һ��


			for(; ;){
				//������
				fd_set rfds;
				FD_ZERO(&rfds);
				FD_SET(lsock, &rfds);

				std::cout << "begin select...\n" << std::endl;

				switch(select(maxfd+1, &rfds, nullptr, nullptr, &timeout)){
					case 0:  //����timeout��û�з���
						std::cout << "timeout...\n" << std::endl;
						break;
					case -1:  //����error����
						std::cout << "select error...\n" << std::endl;
						break;
					default:  //selec"tִ�гɹ��������ļ��������ı�ĸ���
						
						break;
				}
			}
			



			
		}


		~SelectServer(){
			close(lsock);
		}
};