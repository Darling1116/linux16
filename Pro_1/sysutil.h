#ifndef _SYSUTIL_H_
#define _SYSUTIL_H_

#include "common.h"

int tcp_server(const char *host, unsigned short port);//对服务器而言
//先封装一个套接字链接的tcp文件
int tcp_client( );//对客户端而言

const char* statbuf_get_perms(struct stat *sbuf);
const char* statbuf_get_pdate(struct stat *sbuf);

void send_fd(int sock_fd, int fd);
int recv_fd(const int sock_fd);
#endif  /*  _SYSUTIL_H_  */
