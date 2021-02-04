#include "sysutil.h"

int tcp_server (const char *host, unsigned short port){
	int listenfd;
	if((listenfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
		ERR_EXIT("tcp_server");
	//套接字创建失败，就打印错误并退出

	struct sockaddr_in addrSer;
	//服务器地址
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);
	addrSer.sin_addr.s_addr = inet_addr(host);

	int on = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");
	//调动sock的属性opt

	if(bind(listenfd, (struct sockaddr*)&addrSer, sizeof(addrSer)) < 0)
		ERR_EXIT("bind");
	//绑定地址

	if(listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");
	//监听套接字

	return listenfd;
	//返回监听套接字
	}