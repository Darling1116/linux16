#include "privparent.h"
#include "privsock.h"
#include "session.h"

//��ȡ����ģʽ���������׽���
static void privop_pasv_get_data_sock(session_t *sess);

//�ж��Ƿ��ڱ���ģʽ�ļ���״̬
static void privop_pasv_active(session_t *sess);

//��ȡ����ģʽ�µļ����˿�
static void privop_pasv_listen(session_t *sess);

//��ȡ����ģʽ�µ����������׽���
static void privop_pasv_accept(session_t *sess);



void handle_parent(session_t *sess){
	char cmd;
	while(1){
		
		//��ͣ�صȴ�ftp���̵���Ϣ
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

}

static void privop_pasv_listen(session_t *sess){


}


static void privop_pasv_accept(session_t *sess){

}