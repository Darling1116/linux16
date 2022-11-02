#include "sock.hpp"
#include <sys/epoll.h>
#include <stdint.h>


struct epollServer{
	private:
		int lsock; 
		int port;
		int epfd;  //epoll句柄:本质是一个文件描述符

	
	public:
		epollServer(int _port):port(_port){
		}


		void InitepollServer(){
			lsock = Sock::Socket();  //创建套接字
			Sock::Setsockopt(lsock);  //设置端口号为复用状态！
			Sock::Bind(lsock, port);  //绑定端口号
			Sock::Listen(lsock);  //监听套接字

			if((epfd=epoll_create(256)) < 0){
				std::cout << "create epoll error...\n" << std::endl;
				exit(6);
			}
		}


		//告诉内核：关注fd的哪一类事件
		void AddEvent(int fd,  uint32_t event){
			struct epoll_event ev;
			ev.events = 0;
			ev.events |= event;
			ev.data.fd = fd;
			if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)<0){
				std::cerr << "epoll_ctl error...\n" << std::endl;
			}
		}

		//删除的时候不再关心该文件描述符的相关时间了
		void DeleteEvent(int fd){
			if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr)<0){
				std::cerr << "epoll_del error...\n" << std::endl;
			}
		}

		void Start(){
			//添加lsock套接字到epoll中
			AddEvent(lsock, EPOLLIN);
			int time = -1;
			int maxevents = 64;
			struct epoll_event revs[64];  //存放发生的事件
			for(; ; ){
				int num = epoll_wait(epfd, revs, maxevents, time);  //返回值为就绪的事件个数
				if(num > 0){  //有num个事件准备就绪了
					std::cout << "something happen..." << std::endl;
					for(int i=0; i<num; i++){
						int sk= revs[i].data.fd;  //获取该就绪事件的文件描述符
						if(revs[i].events & EPOLLIN){  //有读事件就绪
							if(sk == lsock){  
								struct sockaddr_in peer;
								socklen_t len = sizeof(peer);
								int sock = accept(lsock, (struct sockaddr*)&peer, &len);
								if(sock < 0){
									std::cout << "accept error..." << std::endl;
									continue;
								}
								std::cout << "get a new link..." << std::endl;
								AddEvent(sock, EPOLLIN);  //添加要关注的文件描述符到epoll中
							}
							else{
								char buf[1024];
								ssize_t s = recv(sk, buf, sizeof(buf)-1, 0);  //fd_array[i]里面放的就是套接字
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