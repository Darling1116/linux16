#ifndef _SYSUTIL_H_
#define _SYSUTIL_H_

#include "common.h"

int tcp_server(const char *host, unsigned short port);//�Է���������
//�ȷ�װһ���׽������ӵ�tcp�ļ�
int tcp_client( );//�Կͻ��˶���

const char* statbuf_get_perms(struct stat *sbuf);
const char* statbuf_get_pdate(struct stat *sbuf);


#endif  /*  _SYSUTIL_H_  */
