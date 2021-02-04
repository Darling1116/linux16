#include "session.h"

void begin_session(session_t *sess){
	pid_t pid = fork( );

	if(pid == -1)
		ERR_EXIT("fork");
	if(pid == 0){
		handle_child(sess);	
		//ftp的服务进程
	}
	else{
		handle_parent(sess);
		//nobody进程
	}
}