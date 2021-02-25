#include "privsock.h"
#include "sysutil.h"

//初始化通道
void priv_sock_init(session_t *sess){
	int sockfds[2];
	if(socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0)//创建管道：双向通讯
		ERR_EXIT("socketpair");
	sess->parent_fd = sockfds[0];
	sess->child_fd = sockfds[1];

}

void priv_sock_close(session_t *sess){
	if(sess->parent_fd != -1){
		close(sess->parent_fd);
		sess->parent_fd = -1;
	}
	if(sess->child_fd != -1){
		close(sess->child_fd);
		sess->child_fd = -1;
	}
}


//设置父进程的上下文环境
void priv_sock_set_parent_context(session_t *sess){
	if(sess->child_fd != -1){
		close(sess->child_fd);
		sess->child_fd = -1;
	}
}


void priv_sock_set_child_context(session_t *sess){
	if(sess->parent_fd != -1){
		close(sess->parent_fd);
		sess->parent_fd = -1;
	}
	
}

//发送（接受）命令
void priv_sock_send_cmd(int fd, char cmd){
	int ret = send(fd, &cmd, sizeof(cmd), 0);
	if(ret != sizeof(cmd)){
		ERR_EXIT("priv_sock_send_cmd_error.");
	}
}


char priv_sock_get_cmd(int fd){
	char cmd;
	int ret;
	ret = recv(fd, &cmd, sizeof(cmd), 0);
	if(ret == 0){
		printf("ftp process exit.\n");
		exit(EXIT_SUCCESS);
	}
	if(ret != sizeof(cmd)){
		ERR_EXIT("priv_sock_get_cmd error.");
	}
	return cmd;
}


void priv_sock_send_result(int fd, char res){
	int ret = send(fd, &res, sizeof(res), 0);
	if(ret != sizeof(res)){
		ERR_EXIT("priv_sock_send_result error.");
	}

}

char priv_sock_get_result(int fd){
	char res;
	int ret;
	ret = recv(fd, &res, sizeof(res), 0);
	if(ret == 0){
		printf("ftp process exit.\n");
		exit(EXIT_SUCCESS);
	}
	if(ret != sizeof(res)){
		ERR_EXIT("priv_sock_get_result error.");
	}
	return res;
}


void priv_sock_send_int(int fd, int the_int){
	int ret = send(fd, &the_int, sizeof(the_int), 0);
	if(ret != sizeof(the_int)){
		ERR_EXIT("priv_sock_send_int error.");
	}
}


int priv_sock_get_int(int fd){
	int res;
	int ret;
	ret = recv(fd, &res, sizeof(res), 0);
	if(ret == 0){
		printf("ftp process exit.\n");
		exit(EXIT_SUCCESS);
	}
	if(ret != sizeof(res)){
		ERR_EXIT("priv_sock_get_int error.");
	}
	return res;
}

//发送（接受）字符串
void priv_sock_send_buf(int fd, const char *buf, unsigned int len){
	priv_sock_send_int(fd, len);
	int ret = send(fd, buf, len, 0);
	if(ret != len){
		ERR_EXIT("priv_sock_send_buf error.");
	}

}
void priv_sock_recv_buf(int fd, char *buf, unsigned int len){
	unsigned int recv_len = priv_sock_get_int(fd);
	int ret = recv(fd, buf, recv_len, 0);
	if(ret != recv_len){
		ERR_EXIT("priv_sock_send_buf_error.");
	}
}

void priv_sock_send_fd(int sock_fd, int fd){
	send_fd(sock_fd, fd);
}

int priv_sock_recv_fd(int sock_fd){
	return recv_fd(sock_fd);
}
