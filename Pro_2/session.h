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
	char *rnfr_name;//重命名的名字
	long long restart_pos;
	unsigned int num_clients;
	unsigned int num_per_ip;

	//数据连接（主动）
	struct sockaddr_in *port_addr;
	int data_fd;
	//数据连接（被动）
	int pasv_listen_fd;//被动模式下的监听套接字
	int data_process;

	//父子进程通道
	int child_fd;
	int parent_fd;

	//限速
	unsigned long upload_max_rate;
	unsigned long download_max_rate;
	long transfer_start_sec;
	long transfer_start_usec;

}session_t;

void begin_session(session_t *sess);

#endif
