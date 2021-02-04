#include "ftpproto.h"

void handle_child(session_t *sess){
	
	send(sess->ctrl_fd,"220 (miniftp 3.0.2)\n\r",strlen("220 (miniftp 3.0.2)\n\r"),0);
	//220代表准备就绪
	while(1){
		//不停地等待客户端的命令并进行处理
	}
}