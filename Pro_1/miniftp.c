#include "common.h"
#include "sysutil.h"
#include "session.h"

int main (int argc ,  char *argv[ ]){
	
	//主函数先建立控制连接
	session_t sess = {
		-1
		//初始化内部成员变量
	};
	//sess控制连接内部的成员变量初始化为-1
	

	int listenfd = tcp_server("192.168.109.135",1272);
	//创建一个监听套接字
	
	int sockConn;//新的套接字
	struct sockaddr_in addrCli;//客户端地址
	socklen_t addrlen;//地址长度

	while(1){
		if(sockConn = accept(listenfd,(struct sockaddr*)&addrCli,&addrlen) < 0)
			ERR_EXIT("accept");//接受链接失败

		pid_t pid = fork( );//创建进程

		if(pid == -1)
			ERR_EXIT("fork");//进程创建失败

		if(pid == 0){
			//Child Process
			close(listenfd);//子进程把建立的描述符关掉

			//会话
			sess.ctrl_fd = sockConn;
			begin_session(&sess);
			//相当于主进程中的子进程在创建子进程
			exit(EXIT_SUCCESS);
		}
		else{//pid>0
			//Parent Process
			close(sockConn);//父进程把新建立的套接字关掉
		}
	}
	//主进程永远不停地等待客户端连接
	close(listenfd);//主进程自己关闭所创建的套接字
	return 0;
}
