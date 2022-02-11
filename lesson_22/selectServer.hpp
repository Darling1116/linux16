#include "sock.hpp"

#include <sys/select.h>

#define NUM (sizeof(fd_set)*8)  //定义辅助数组的大小为8个比特位
#define DFL_FD -1  //定义默认的文件描述符的状态为-1

struct SelectServer{
	private:
		int lsock; //监听套接字
		int port;
		int fd_array[NUM];  //辅助数组：储存fd_set *rfds的内容，把所有文件描述符都先储存在该数组里面
	
	public:
		SelectServer(int _port):port(_port){
		}


		void InitSelectServer(){
			for(int i=0; i<NUM; i++){
				fd_array[i] = DFL_FD;
			}

			lsock = Sock::Socket();  //创建套接字
			Sock::Setsockopt(lsock);  //设置端口号为复用状态！
			Sock::Bind(lsock, port);  //绑定端口号
			Sock::Listen(lsock);  //监听套接字

			fd_array[0] = lsock;  //定义0号下标存放的是listen套接字
		}

		
		void AddFdToArray(int sock){
			int i=0;
			for ( ; i<NUM; i++){
				if(fd_array[i] == DFL_FD){
					break;
				}
			}

			if(i >= NUM){  //数组已满，无法添加
				std::cout << "fd_array is full, close sock...\n" << std::endl;
				close(sock);
			}
			else{
				fd_array[i] = sock;
				std::cout << "fd: " << sock << "add to select...\n" << std::endl;
			}
		}


		void HandlerEvent(fd_set *rfds){
			for(int i=0; i<NUM; i++){
				if(fd_array[i] == DFL_FD){  //不是合法文件描述符
					continue;
				}  

				//检测合法的文件描述符
				if(FD_ISSET(fd_array[i], rfds)){
					//读事件就绪
					//1.listen连接事件就绪
					if(fd_array[i] == lsock){
						int sock = Sock::Accept(lsock);
						if(sock >= 0){
							std::cout << "get a new link...\n" << std::endl;
							//数据通信连接建立成功，但不能直接开始通讯，可能会阻塞挂起
							//把数据通讯连接的套接字放入fd_array辅助数组中
							AddFdToArray(sock);
						}
					}
					//2.读数据事件就绪
					else{

					}
				}
			}
		}



		void Start(){
			//在服务器刚启动时只有一个listen状态下的文件描述符
			//int maxfd = lsock;  
			int maxfd = DFL_FD;
			//每隔5秒检测一次：此后timeout会变成0，立即返回，不等待外部事件的发生，为非阻塞轮询
			//struct timeval timeout = {5, 0};  

			for(; ;){
				//---读操作---
				fd_set rfds;
				//对select中的参数进行了重新设定！！！
				FD_ZERO(&rfds);
				std::cout << "fd_array: ";

				for(int i=0; i<NUM; i++){
					if(fd_array[i] != DFL_FD){
						std::cout << fd_array[i] << " ";
						FD_SET(fd_array[i], &rfds);  //把有效的文件描述符放入rfds中
						if(maxfd < fd_array[i]){  //更新最大文件描述符值
							maxfd = fd_array[i];
						}
					}
				}
				std:cout << std::endl;
				//FD_SET(lsock, &rfds);
				std::cout << "begin select...\n" << std::endl;

				//struct timeval timeout = {5, 0};  //每次select的等待时间都为5秒，为阻塞轮询
				//timeout为空，表示select一直被阻塞，直到某个文件描述符状态发生了变化
				switch(select(maxfd+1, &rfds, nullptr, nullptr, /*timeout*/ nullptr)){
					case 0:  //超过timeout，没有返回
						std::cout << "timeout...\n" << std::endl;
						break;
					case -1:  //发生error错误
						std::cout << "select error...\n" << std::endl;
						break;
					default:  //select执行成功，返回文件描述符改变的个数
						HandlerEvent(&rfds);
						break;
				}
			}	

		}


		~SelectServer(){
			close(lsock);
		}
};