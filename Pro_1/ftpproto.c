#include "ftpproto.h"

void handle_child(session_t *sess){
	
	send(sess->ctrl_fd,"220 (miniftp 3.0.2)\n\r",strlen("220 (miniftp 3.0.2)\n\r"),0);
	//220����׼������
	while(1){
		//��ͣ�صȴ��ͻ��˵�������д���
	}
}