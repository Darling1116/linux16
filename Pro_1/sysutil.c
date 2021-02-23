#include "sysutil.h"

int tcp_server (const char *host, unsigned short port){
	int listenfd;
	if((listenfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
		ERR_EXIT("tcp_server");
	//�׽��ִ���ʧ�ܣ��ʹ�ӡ�����˳�

	struct sockaddr_in addrSer;
	//��������ַ
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);
	addrSer.sin_addr.s_addr = inet_addr(host);

	int on = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");
	//����sock������opt

	if(bind(listenfd, (struct sockaddr*)&addrSer, sizeof(addrSer)) < 0)
		ERR_EXIT("bind");
	//�󶨵�ַ

	if(listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");
	//�����׽���

	return listenfd;
	//���ؼ����׽���
}



int tcp_client( ){
	int sock;//�����׽���
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
	strftime(datebuf, 64, "%b %e %H:%M", ptm);//ʱ���ʽ��
	return datebuf;
}