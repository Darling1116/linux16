#ifndef _SYSUTIL_H_
#define _SYSUTIL_H_

#include "common.h"

int tcp_server(const char *host, unsigned short port);//�Է���������
//�ȷ�װһ���׽������ӵ�tcp�ļ�
int tcp_client(int port);//�Կͻ��˶���

const char* statbuf_get_perms(struct stat *sbuf);
const char* statbuf_get_pdate(struct stat *sbuf);

void send_fd(int sock_fd, int fd);
int recv_fd(const int sock_fd);

long get_time_sec();
long get_time_usec();

#endif  /*  _SYSUTIL_H_  */
