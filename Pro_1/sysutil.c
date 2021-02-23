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



int tcp_client( ){
	int sock;//创建套接字
	if((sock = socket( AF_INET, SOCK_STREAM, 0)) < 0){
		ERR_EXIT("tcp_client");
	}
	/*struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htohs(20);
	bind(sock,(struct sockaddr*)&address, sizeof(struct sockaddr)) < 0)
		ERR_EXIT("bind 20");
	*/
	return sock;
}


const char *statbuf_get_perms(struct stat *sbuf){
	static char perms[] = "--------------";
	mode_t mode = sbuf->st_mode;
	switch(mode & S_IFMT){
		case S_IFSOCK:
			perms[0] = 's';
			break;
		case S_IFLNK:
			perms[0] = 'l';
			break;
		case S_IFREG:
			perms[0] = '-';
			break;
		case S_IFBLK:
			perms[0] = 'b';
			break;
		case S_IFDIR:
			perms[0] = 'd';
			break;
		case S_IFCHR:
			perms[0] = 'c';
			break;
		case S_IFIFO:
			perms[0] = 'p';
			break;
	}
	if(mode & S_IRUSR)
		perms[1] = 'r';
	if(mode & S_IWUSR)
		perms[1] = 'w';
	if(mode & S_IXUSR)
		perms[1] = 'x';

	if(mode & S_IRGRP)
		perms[1] = 'r';
	if(mode & S_IWGRP)
		perms[1] = 'w';
	if(mode & S_IXGRP)
		perms[1] = 'x';

	if(mode & S_IROTH)
		perms[1] = 'r';
	if(mode & S_IWOTH)
		perms[1] = 'w';
	if(mode & S_IXOTH)
		perms[1] = 'x';
	
	return perms;
}


const char *statbuf_get_date(struct stat *sbuf){
	static char datebuf[64] = {0};
	time_t file_time = sbuf->st_mtime;
	struct tm *ptm = localtime(&file_time);
	strftime(datebuf, 64, "%b %e %H:%M", ptm);//时间格式化
	return datebuf;
}


void send_fd(int sock_fd, int fd){
	int ret;
	struct msghdr msg;
	struct cmsghdr *p_cmsg;
	struct iovec vec;
	char cmsgbuf[CMSG_SPACE(sizeof(fd))];
	int *p_fds;
	char sendchar = 0;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	p_cmsg = CMSG_FIRSTHDR(&msg);
	p_cmsg->cmsg_level = SOL_SOCKET;
    p_cmsg->cmsg_type = SCM_RIGHTS;
	p_cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
	p_fds = (int*)CMSG_DATA(p_cmsg);
	*p_fds = fd;
	
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;

	vec.iov_base = &sendchar;
	vec.iov_len = sizeof(sendchar);
	ret = sendmsg(sock_fd, &msg, 0);
		ERR_EXIT("sendmsg");
}


int recv_fd(const int sock_fd){
	int ret;
	struct msghdr msg;
	char recvchar;
	struct iovec vec;
	int recv_fd;
	char cmsgbuf[CMSG_SPACE(sizeof(recv_fd))];
	struct cmsghdr *p_cmsg;
	int *p_fd;
	
	vec.iov_base = &recvchar;
	vec.iov_len = sizeof(recvchar);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);

	p_fd = (int*)CMSG_DATA(CMSG_FIRSTHDR(&msg));
	*p_fd = -1;
	ret = recvmsg(sock_fd, &msg, 0);
	if(ret != 1)
		ERR_EXIT("recvmsg");

	p_cmsg = CMSG_FIRSTHDR(&msg);
	if(p_cmsg == NULL)
		ERR_EXIT("no passed fd");

	p_fd = (int*)CMSG_DATA(p_cmsg);
	recv_fd = *p_fd;
	if(recv_fd == -1)
		ERR_EXIT("no passed fd");

	return recv_fd;
}