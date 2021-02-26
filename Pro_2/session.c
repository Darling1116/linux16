#include "session.h"

void begin_session(session_t *sess){

	//初始化父子进程通道
	priv_sock_init(sess);
	/*int sockfds[2];
	if(socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0)
		ERR_EXIT("socketpair");
	sess->child_fd = sockfds[1];
	sess->parent_fd = sockfds[0];
	*/


	pid_t pid = fork( );

	if(pid == -1)
		ERR_EXIT("fork");
	if(pid == 0){
		priv_sock_set_child_context(sess);
		/*
		close(sess->parent_fd);//子进程创建成功后，关闭掉原来的父进程
		sess->parent_fd = -1;
		*/
		//ftp的服务进程
		handle_child(sess);	   
	}
	else{
	    //nobody进程
		priv_sock_set_parent_context(sess);
		/*
		close(sess->chlid_fd);//nobody进程创建成功后，关闭掉原来的子进程
		sess->child_fd = -1;
		*/

		//把root进程更改为nobody---见minimize_privilege()函数
	    //把root进程更改进程名为nobody

		handle_parent(sess);   
	}
}