#include "sock.hpp"
#include <sys/epoll.h>
#include <stdint.h>


struct epollServer{
	private:
		int lsock; 
		int port;
		int epfd;  //epoll���:������һ���ļ�������

	
	public:
		epollServer(int _port):port(_port){
		}


		void InitepollServer(){
			lsock = Sock::Socket();  //�����׽���
			Sock::Setsockopt(lsock);  //���ö˿ں�Ϊ����״̬��
			Sock::Bind(lsock, port);  //�󶨶˿ں�
			Sock::Listen(lsock);  //�����׽���

			if((epfd=epoll_create(256)) < 0){
				std::cout << "create epoll error...\n" << std::endl;
				exit(6);
			}
		}


		//�����ںˣ���עfd����һ���¼�
		void AddEvent(int fd,  uint32_t event){
			struct epoll_event ev;
			ev.events = 0;
			ev.events |= event;
			ev.data.fd = fd;
			if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)<0){
				std::cerr << "epoll_ctl error...\n" << std::endl;
			}
		}

		//ɾ����ʱ���ٹ��ĸ��ļ������������ʱ����
		void DeleteEvent(int fd){
			if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr)<0){
				std::cerr << "epoll_del error...\n" << std::endl;
			}
		}

		void Start(){
			//���lsock�׽��ֵ�epoll��
			AddEvent(lsock, EPOLLIN);
			int time = -1;
			int maxevents = 64;
			struct epoll_event revs[64];  //��ŷ������¼�
			for(; ; ){
				int num = epoll_wait(epfd, revs, maxevents, time);  //����ֵΪ�������¼�����
				if(num > 0){  //��num���¼�׼��������
					std::cout << "something happen..." << std::endl;
					for(int i=0; i<num; i++){
						int sk= revs[i].data.fd;  //��ȡ�þ����¼����ļ�������
						if(revs[i].events & EPOLLIN){  //�ж��¼�����
							if(sk == lsock){  
								struct sockaddr_in peer;
								socklen_t len = sizeof(peer);
								int sock = accept(lsock, (struct sockaddr*)&peer, &len);
								if(sock < 0){
									std::cout << "accept error..." << std::endl;
									continue;
								}
								std::cout << "get a new link..." << std::endl;
								AddEvent(sock, EPOLLIN);  //���Ҫ��ע���ļ���������epoll��
							}
							else{
								char buf[1024];
								ssize_t s = recv(sk, buf, sizeof(buf)-1, 0);  //fd_array[i]����ŵľ����׽���
								if(s > 0){
									buf[s]= 0;
									std::cout << "client# " << buf << std::endl;
								}
								else{
									std::cout << "recv data error...\n" << std::endl;
									close(sk);
									DeleteEvent(sk);
								}
							}
						}
					}
				
				}
				else if(num==0){
					std::cout << "timeout" << std::endl;
				}
				else{
					std::cout << "epoll wait error" << std::endl;
				}

			}
		}

		~epollServer(){
			close(lsock);
			if(epfd>=0){
				close(epfd);
			}
		}
};