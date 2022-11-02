#include "sock.hpp"

#include <sys/select.h>

#define NUM (sizeof(fd_set)*8)  //���帨������Ĵ�СΪ8������λ
#define DFL_FD -1  //����Ĭ�ϵ��ļ���������״̬Ϊ-1

struct SelectServer{
	private:
		int lsock; //�����׽���
		int port;
		int fd_array[NUM];  //��ŵ�����Ҫ�ȴ���fd
		//�������飺����fd_set *rfds�����ݣ��������ļ����������ȴ����ڸ���������
	
	public:
		SelectServer(int _port):port(_port){
		}


		void InitSelectServer(){
			for(int i=0; i<NUM; i++){
				fd_array[i] = DFL_FD;
			}

			lsock = Sock::Socket();  //�����׽���
			Sock::Setsockopt(lsock);  //���ö˿ں�Ϊ����״̬��
			Sock::Bind(lsock, port);  //�󶨶˿ں�
			Sock::Listen(lsock);  //�����׽���

			fd_array[0] = lsock;  //����0���±��ŵ���listen�׽���
		}

		
		void AddFdToArray(int sock){
			int i=0;
			for ( ; i<NUM; i++){
				if(fd_array[i] == DFL_FD){
					break;
				}
			}

			if(i >= NUM){  //�����������޷����
				std::cout << "fd_array is full, close sock...\n" << std::endl;
				close(sock);
			}
			else{
				fd_array[i] = sock;
				std::cout << "fd: " << sock << " add to select...\n" << std::endl;
			}
		}


		void DefFdFromArray(int index){  //�����ļ���������fd_array�е��±�λ��
			if(index>=0 && index<NUM){
				fd_array[index] = DFL_FD;
			}
		}


		void HandlerEvent(fd_set *rfds){
			for(int i=0; i<NUM; i++){

				if(fd_array[i] == DFL_FD){  //���ǺϷ��ļ�������
					continue;
				}  

				//���Ϸ����ļ�������
				if(FD_ISSET(fd_array[i], rfds)){
					//���¼�����
					//1.listen�����¼�����
					if(fd_array[i] == lsock){
						int sock = Sock::Accept(lsock);
						if(sock >= 0){
							std::cout << "get a new link...\n" << std::endl;
							//����ͨ�����ӽ����ɹ���������ֱ�ӿ�ʼͨѶ�����ܻ���������
							//������ͨѶ���ӵ��׽��ַ���fd_array����������
							AddFdToArray(sock);
						}
					}

					//2.�������¼�����
					//�����ǰ����ݴ��ں˴����û�
					else{
						char buf[1024];
						ssize_t s = recv(fd_array[i], buf, sizeof(buf), 0);  //fd_array[i]����ŵľ����׽���
						if(s > 0){
							std::cout << "client# " << buf << std::endl;
						}
						else if(s == 0){  
							std::cout << "client quit...\n" << std::endl;
							close(fd_array[i]);
							DefFdFromArray(i);  //�Ѹ��ļ��������Ӹ���������ɾ��
						}
						else{
							std::cout << "recv data error...\n" << std::endl;
							close(fd_array[i]);
						}
					}
				}
			}
		}



		void Start(){
			//�ڷ�����������ʱֻ��һ��listen״̬�µ��ļ�������
			//int maxfd = lsock;  
			int maxfd = DFL_FD;
			//ÿ��5����һ�Σ��˺�timeout����0���������أ����ȴ��ⲿ�¼��ķ�����Ϊ��������ѯ
			struct timeval timeout = {5, 0};  

			for(;;){
				
				//---������---
				fd_set rfds;
				//��select�еĲ��������������趨������
				FD_ZERO(&rfds);
				std::cout << "fd_array: ";

				for(int i=0; i<NUM; i++){
					if(fd_array[i] != DFL_FD){
						std::cout << fd_array[i] << " ";
						FD_SET(fd_array[i], &rfds);  //����Ч���ļ�����������rfds��
						if(maxfd < fd_array[i]){  //��������ļ�������ֵ
							maxfd = fd_array[i];
						}
					}
				}
				std:cout << "\n" << std::endl;
				//FD_SET(lsock, &rfds);
	
				std::cout << "begin select...\n" << std::endl;
				//struct timeval timeout = {10, 0};  //ÿ��select�ĵȴ�ʱ�䶼Ϊ5�룬Ϊ������ѯ
				//timeoutΪ�գ���ʾselectһֱ��������ֱ��ĳ���ļ�������״̬�����˱仯
				switch(select(maxfd+1, &rfds, nullptr, nullptr, &timeout)){
					case 0:  //����timeout��û�з���
						std::cout << "timeout...\n" << std::endl;
						break;
					case -1:  //����error����
						std::cout << "select error...\n" << std::endl;
						break;
					default:  //selectִ�гɹ��������ļ��������ı�ĸ���
						HandlerEvent(&rfds);  //�ȴ�������׼������������
						break;
				}
			}	

		}


		~SelectServer(){
			close(lsock);
		}
};