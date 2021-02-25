#include "privparent.h"
#include "privsock.h"
#include "session.h"

//获取主动模式数据连接套接字
static void privop_pasv_get_data_sock(session_t *sess);

//判断是否处于被动模式的激活状态
static void privop_pasv_active(session_t *sess);

//获取被动模式下的监听端口
static void privop_pasv_listen(session_t *sess);

//获取被动模式下的数据连接套接字
static void privop_pasv_accept(session_t *sess);



void handle_parent(session_t *sess){
	char cmd;
	while(1){
		
		//不停地等待ftp进程的消息
		cmd = priv_sock_get_cmd(sess->parent_fd);
		switch(cmd){
			case PRIV_SOCK_GET_DATA_SOCK:
				privop_pasv_get_data_sock(sess);
				break;
			case PRIV_SOCK_PASV_ACTIVE:
				privop_pasv_active(sess);
				break;
			case PRIV_SOCK_PASV_LISTEN:
                privop_pasv_listen(sess);
				break;
			case PRIV_SOCK_PASV_ACCEPT:
				privop_pasv_accept(sess);
				break;
		}
	}
}


static void privop_pasv_get_data_sock(session_t *sess){
	unsigned short port = (unsigned short)priv_sock_get_int(sess->parent_fd);
	char ip[16] = {0};
	priv_sock_recv_buf(sess->parent_fd, ip, sizeof(ip));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip);

	int fd = tcp_client();
	if(fd == -1){
		priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
		return;
	}

	if(connect(fd, (struct sockaddr*)&address, sizeof(address)) < 0){
		close(fd);
		priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
		return;
	}

	priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_OK);
	priv_sock_send_fd(sess->parent_fd, fd);
	close(fd);
}


static void privop_pasv_active(session_t *sess){
	int active;
	if(sess->pasv_listen_fd != -1)
		active = 1;
	else
		active = 0;
	priv_sock_send_int(sess->parent_fd, active);
}

static void privop_pasv_listen(session_t *sess){
	char *ip = "192.168.109.138";
	sess->pasv_listen_fd = tcp_server(ip, 0);//端口0表示绑定临时端口

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(struct sockaddr);
	if(getsockname(sess->pasv_listen_fd, (struct sockaddr*)&address, &addrlen) < 0)
		ERR_EXIT("getsockname");

	unsigned short port = ntohs(address.sin_port);//网络字节区转化为本地字节区
	priv_sock_send_int(sess->parent_fd, (int)port);
}


static void privop_pasv_accept(session_t *sess){
	int fd = accept(sess->pasv_listen_fd, 0, 0);
	close(sess->pasv_listen_fd);
	sess->pasv_listen_fd = -1;

	if(fd == -1){
		priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
		return;
	}
	
	priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_OK);
	priv_sock_send_fd(sess->parent_fd, fd);
	close(fd);

}