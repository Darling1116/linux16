#ifndef _SESSION_H_
#define _SESSION_H_

#include"common.h"

typedef struct session
{
	//��������
	uid_t uid;
	int ctrl_fd;
	char cmdline[MAX_COMMAND_LINE];//������
	char cmd[MAX_COMMAND];
	char arg[MAX_ARG];

	//ftp��Э��״̬
	int is_ascii;

	//�������ӣ�������
	struct sockaddr_in *port_addr;
	int data_fd;
	//�������ӣ�������
	int pasv_listen_fd;//����ģʽ�µļ����׽���

}session_t;

void begin_session(session_t *sess);

#endif
