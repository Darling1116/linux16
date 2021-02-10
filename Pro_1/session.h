#ifndef _SESSION_H_
#define _SESSION_H_

#include"common.h"

typedef struct session
{
	//控制连接
	uid_t uid;
	int ctrl_fd;
	char cmdline[MAX_COMMAND_LINE];//命令行
	char cmd[MAX_COMMAND];
	char arg[MAX_ARG];
}session_t;

void begin_session(session_t *sess);

#endif
