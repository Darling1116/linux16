#include "common.h"
#include "sysutil.h"
#include "session.h"

int main (int argc ,  char *argv[ ]){
	
	//�������Ƚ�����������
	session_t sess = {
		-1
		//��ʼ���ڲ���Ա����
	};
	//sess���������ڲ��ĳ�Ա������ʼ��Ϊ-1
	

	int listenfd = tcp_server("192.168.109.135",1272);
	//����һ�������׽���
	
	int sockConn;//�µ��׽���
	struct sockaddr_in addrCli;//�ͻ��˵�ַ
	socklen_t addrlen;//��ַ����

	while(1){
		if(sockConn = accept(listenfd,(struct sockaddr*)&addrCli,&addrlen) < 0)
			ERR_EXIT("accept");//��������ʧ��

		pid_t pid = fork( );//��������

		if(pid == -1)
			ERR_EXIT("fork");//���̴���ʧ��

		if(pid == 0){
			//Child Process
			close(listenfd);//�ӽ��̰ѽ������������ص�

			//�Ự
			sess.ctrl_fd = sockConn;
			begin_session(&sess);
			//�൱���������е��ӽ����ڴ����ӽ���
			exit(EXIT_SUCCESS);
		}
		else{//pid>0
			//Parent Process
			close(sockConn);//�����̰��½������׽��ֹص�
		}
	}
	//��������Զ��ͣ�صȴ��ͻ�������
	close(listenfd);//�������Լ��ر����������׽���
	return 0;
}
