#include "session.h"

void begin_session(session_t *sess){
	pid_t pid = fork( );

	if(pid == -1)
		ERR_EXIT("fork");
	if(pid == 0){
		handle_child(sess);	
		//ftp�ķ������
	}
	else{
		handle_parent(sess);
		//nobody����
	}
}