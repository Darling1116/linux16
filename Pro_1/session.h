#ifndef _SESSION_H_
#define _SESSION_H_

#include"common.h"

typedef struct session
{
	//控制连接
	uid_t uid;
	int ctrl_fd;
	char cmdline[MAX_COMMAND_LINE];//命令行
	char cmd[MAX_COMMAND];
	char arg[MAX_ARG];

	//ftp的协议状态
	int is_ascii;

	//数据连接（主动）
	struct sockaddr_in *port_addr;
	int data_fd;
	//数据连接（被动）
	int pasv_listen_fd;//被动模式下的监听套接字

}session_t;

void begin_session(session_t *sess);

#endif
