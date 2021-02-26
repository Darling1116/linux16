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
	char *rnfr_name;//������������
	long long restart_pos;
	unsigned int num_clients;
	unsigned int num_per_ip;

	//�������ӣ�������
	struct sockaddr_in *port_addr;
	int data_fd;
	//�������ӣ�������
	int pasv_listen_fd;//����ģʽ�µļ����׽���
	int data_process;

	//���ӽ���ͨ��
	int child_fd;
	int parent_fd;

	//����
	unsigned long upload_max_rate;
	unsigned long download_max_rate;
	long transfer_start_sec;
	long transfer_start_usec;

}session_t;

void begin_session(session_t *sess);

#endif
