#include "sock.hpp"

#include <sys/select.h>

struct SelectServer{
	private:
		int lsock; //监听套接字
		int port;
	
	public:
		SelectServer(int _port):port(_port){
		}


		void InitSelectServer(){
			lsock = Sock::Socket();  //创建套接字
			Sock::Setsockopt(lsock);  //设置端口号为复用状态！
			Sock::Bind(lsock, port);  //绑定端口号
			Sock::Listen(lsock);  //监听套接字
		}

		void Start(){
			//在服务器刚启动时只有一个listen状态下的文件描述符
			int maxfd = lsock;  
			struct timeval timeout = {1000, 0};  //每隔5秒检测一次


			for(; ;){
				//读操作
				fd_set rfds;
				FD_ZERO(&rfds);
				FD_SET(lsock, &rfds);

				std::cout << "begin select...\n" << std::endl;

				switch(select(maxfd+1, &rfds, nullptr, nullptr, &timeout)){
					case 0:  //超过timeout，没有返回
						std::cout << "timeout...\n" << std::endl;
						break;
					case -1:  //发生error错误
						std::cout << "select error...\n" << std::endl;
						break;
					default:  //selec"t执行成功，返回文件描述符改变的个数
						
						break;
				}
			}
			



			
		}


		~SelectServer(){
			close(lsock);
		}
};