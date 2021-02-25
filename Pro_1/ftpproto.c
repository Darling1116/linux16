#include "ftpproto.h"
#include "ftpcodes.h"
#include "str.h"
#include "sysutil.h"
#include "privsock.h"

void ftp_reply(session_t *sess, int code, const char *text){
	char buf[MAX_BUFFER_SIZE] = {0};
	sprintf(buf, "%d %s\r\n", code, text);
	send(sess->ctrl_fd, buf, strlen(buf), 0);
}



////////命令映射机制:把命令映射到指定函数中去执行/////////

static void do_user(session_t *sess);
static void do_pass(session_t *sess);
static void do_syst(session_t *sess);//获取系统信息
static void do_feat(session_t *sess);//获取服务器特性
static void do_pwd(session_t *sess);//获取工作路径
static void do_type(session_t *sess);
static void do_port(session_t *sess);//主动模式
static void do_list(session_t *sess);////获取列表信息
static void do_pasv(session_t *sess);//被动模式

typedef struct ftpcmd{
	const char *cmd;
	void(*cmd_handler)(session_t *sess);
}ftpcmd_t;

//命令映射表
static ftpcmd_t ctrl_cmds[] = {
	{"USER", do_user},
	{"PASS", do_pass},
	{"SYST", do_syst},
	{"FEAT", do_feat},
	{"PWD", do_pwd},
	{"TYPE", do_type},
	{"PORT", do_port},
	{"LIST", do_list},
	{"PASV", do_pasv}
	
};
//捆绑命令和函数




//ftp服务进程
void handle_child(session_t *sess){
	//send(sess->ctrl_fd,"220 (miniftp 3.0.2)\n\r",strlen("220 (miniftp 3.0.2)\n\r"),0);
	//220代表准备就绪
	ftp_reply(sess, FTP_GREET, "(myminiftp 1.0)");
	
	int ret;
	while(1){
	//不停地等待客户端的命令并进行处理：接收命令
		memset(sess->cmdline, 0, MAX_COMMAND_LINE);
		memset(sess->cmd, 0, MAX_COMMAND);
		memset(sess->arg, 0, MAX_ARG);
		ret = recv(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE, 0);
		if(ret == -1)
			ERR_EXIT("readline");
		else if (ret == 0)
			exit(EXIT_SUCCESS);
		
		str_trim_crlf(sess->cmdline);
		//printf("cmdline = %s\n", sess->cmdline);
		//接收数据---数据的组成：命令+空格+参数
    	str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
		//分割命令行
			//printf("cmd = %s\n",sess->cmd);
			//printf("arg = %s\n",sess->arg);
		

		int table_size = sizeof(ctrl_cmds) / sizeof(ftpcmd_t);
		int i;
		for(i=0; i<table_size; ++i){
			if(strcmp(sess->cmd, ctrl_cmds[i].cmd) == 0){
				if(ctrl_cmds[i].cmd_handler != NULL){
					ctrl_cmds[i].cmd_handler(sess);
				}
				else{
					ftp_reply(sess, FTP_COMMANDOTIMPL, "Unimplement command.");
				}
				break;
			}
		}

		if(i >= table_size)
			ftp_reply(sess, FTP_BADCMD, "Unknow command.");

	}
}


//检查用户名是否匹配
//USER  gxr116
static void do_user(session_t *sess){
	struct passwd *pwd = getpwnam(sess->arg);
	/*if(pwd == NULL){
		ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
		return;
	}
	sess->uid = pwd->pw_uid;
	*/

	if(pwd != NULL){
		sess->uid = pwd->pw_uid;
	}

	ftp_reply(sess, FTP_GIVEPWDRD, "please specify the password");
}


//鉴权:用户在存在的情况下，保存用户
//PASS 123456
static void do_pass(session_t *sess){
	struct passwd *pwd = getpwuid(sess->uid);
	if(pwd == NULL){
		ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
		return;
	}

	struct spwd *spd = getspnam(pwd->pw_name);
	//根据用户名获取用户的影子文件
	if(spd == NULL){
		ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
		return;
	}
	char *encry_pwd = crypt(sess->arg, spd->sp_pwdp);
	//printf("encry_pwd = %s\n", encry_pwd);
	if(strcmp(encry_pwd, spd->sp_pwdp) != 0){
		ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
		return;
	}
	
	//密码成功后，修改uid
	setegid(pwd->pw_gid);
	seteuid(pwd->pw_uid);
	chdir(pwd->pw_dir);
	
	ftp_reply(sess, FTP_LOGINOK, "Login successful.");
}

//获取系统信息
static void do_syst(session_t *sess){
	ftp_reply(sess, FTP_SYSTOK, "UNIX Type: L8");
}

//获取服务器特性
static void do_feat(session_t *sess){
	//ftp_reply(sess, FTP_BADCMD, "-Features:");
    send(sess->ctrl_fd, "211-Features:\r\n", strlen("211-Features:\r\n"), 0);
	send(sess->ctrl_fd, "ERPT\r\n", strlen("ERPT\r\n"), 0);
	send(sess->ctrl_fd, "EPSV\r\n", strlen("EPSV\r\n"), 0);
	send(sess->ctrl_fd, "MDTM\r\n", strlen("MDTM\r\n"), 0);
	send(sess->ctrl_fd, "REST STREAM\r\n", strlen("REST STREAM\r\n"), 0);
	send(sess->ctrl_fd, "SIZE\r\n", strlen("SIZE\r\n"), 0);
	send(sess->ctrl_fd, "TVFS\r\n", strlen("TVFS\r\n"), 0);
	send(sess->ctrl_fd, "UTF8\r\n", strlen("UTF8\r\n"), 0);
	//ftp_reply(sess, FTP_BADCMD, "End");
    send(sess->ctrl_fd, "211 End\r\n", strlen("211 End\r\n"), 0);
}

//获取当前的工作路径（当前的宿主目录）
static void do_pwd(session_t *sess){
	char buffer[MAX_BUFFER_SIZE] = {0};
	getcwd(buffer, MAX_BUFFER_SIZE);//获取当前宿主目录的函数
	char msg[MAX_BUFFER_SIZE] = {0};
	sprintf(msg, "\"%s\"", buffer);
	ftp_reply(sess, FTP_PWDOK, msg);
}

//获取当前ftp传输模式的编码类型
static void do_type(session_t *sess){
	if(strcmp(sess->arg, "A") == 0){//ASCII模式
		sess->is_ascii = 1;
        ftp_reply(sess, FTP_TYPEOK, "Switching to ASCII mode.");
	}

	else if(strcmp(sess->arg, "I") == 0){//二进制模式
		sess->is_ascii = 0;
        ftp_reply(sess, FTP_TYPEOK, "Switching to Binary mode.");
	}

	else{
		ftp_reply(sess, FTP_BADCMD, "Unrecongnised TYPE command.");
	}
}




//判断主动模式是否被激活
int port_active(session_t *sess){
	if(sess->port_addr){/////////
		if(pasv_active(sess)){
			fprintf(stderr,"both port and pasv are active");
			exit(EXIT_FAILURE);
		}
		return 1;
	}
	return 0;
}


//判断被动模式是否被激活
int pasv_active(session_t *sess){

	priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_ACTIVE);

	if(priv_sock_get_int(sess->child_fd)){
		if(port_active(sess)){
			fprintf(stderr,"both port and pasv are active");
			exit(EXIT_FAILURE);
		}
		return 1;
	}
	return 0;//没有被激活
}



//获取数据连接的方法
int get_transfer_fd(session_t *sess){
	if(!port_active(sess) && !pasv_active(sess)){
		ftp_reply(sess, FTP_BADSENDCONN, "Use Port or Pasv first.");
		return 0;
	}

	int ret = 1;
	if(port_active(sess)){

	    /*
		int sock = tcp_client( );//主动连接：客户端创建套接字////////
		if(connect(sock, (struct sockaddr*)sess->port_addr, sizeof(struct sockaddr)) < 0){
			ret = 0;//连接套接字
		}
		else{
			sess->data_fd = sock;
		}
		*/

		if(!get_port_fd(sess))
			ret = 0;
			
			
	}


	if(pasv_active(sess)){
		/*
		int sock = accept(sess->pasv_listen_fd, NULL, NULL);//被动连接：客户端接受套接字
		if(sock < 0)
			ret = 0;
		else{
			close(sess->pasv_listen_fd);
			sess->pasv_listen_fd = -1;
			sess->data_fd = sock;
		}
		*/
		if(!get_pasv_fd(sess))
			ret = 0;
	}
	
    if(sess->port_addr){
		free(sess->port_addr);
		sess->port_addr = NULL;
	}

	return ret;
}



int get_port_fd(session_t *sess){
	int ret = 1;
////////    ftp服务进程向nobody进程发起请求（连接）

	priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_GET_DATA_SOCK);
	unsigned short port = ntohs(sess->port_addr->sin_port);
	char *ip = inet_ntoa(sess->port_addr->sin_addr);

	////////	发送port
	priv_sock_send_int(sess->child_fd, (int)port);
	priv_sock_send_buf(sess->child_fd, ip, strlen(ip));

	char res = priv_sock_get_result(sess->child_fd);
	if(res == PRIV_SOCK_RESULT_BAD)
		ret = 0;
	else if(res == PRIV_SOCK_RESULT_OK)
		sess->data_fd = priv_sock_recv_fd(sess->child_fd);
	
	return ret;
}


int get_pasv_fd(session_t *sess){
	int ret = 1;
	priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_ACCEPT);
	char res = priv_sock_get_result(sess->child_fd);
	if(res == PRIV_SOCK_RESULT_BAD){
		ret = 0;
	}
	else if(res == PRIV_SOCK_RESULT_OK){
  		sess->data_fd = priv_sock_recv_fd(sess->child_fd);
	}
	return ret;
}


static void list_common(session_t *sess){
	DIR *dir = opendir(".");//打开当前目录
	if(dir == NULL)
		return;


	char buf[MAX_BUFFER_SIZE]  = {0};

	struct stat sbuf;//保存文件的属性
	struct dirent *dt;

	while((dt = readdir(dir)) != NULL){
		if(stat(dt->d_name, &sbuf) < 0)
			continue;
	    if(dt->d_name[0] == '.')//过滤掉隐藏文件
			continue;
		
		memset(buf, MAX_BUFFER_SIZE, 0);
	
		//先组合权限
		const char *perms = statbuf_get_perms(&sbuf);
		int offset = 0;
		offset += sprintf(buf, "%s", perms);
		offset += sprintf(buf+offset, "%3d %-8d %-8d %8u", sbuf.st_nlink, sbuf.st_uid, sbuf.st_gid, sbuf.st_size);

		//后组合时间和日期
		const char *pdate = statbuf_get_date(&sbuf);
		offset += sprintf(buf+offset, "%s ", pdate);
		sprintf(buf+offset, "%s\r\n", dt->d_name);
	
		//发送数据
		send(sess->data_fd, buf, strlen(buf), 0);
	}
}
	


static void do_list(session_t *sess){
//1.建立数据连接（担当主动连接和被动连接）
	if(get_transfer_fd(sess) == 0)
		return;

//2.回复150
	ftp_reply(sess, FTP_DATACINN, "Here comes the directory listing.");

//3.显示列表
	//send(sess->data_fd, "drwxr-xr-x  2 gxr116 gxr116  6 1月  22 22:36 Desktop\r\n", strlen("drwxr-xr-x  2 gxr116 gxr116  6 1月  22 22:36 Desktop\r\n"), 0);
	//send(sess->data_fd, "drwxr-xr-x. 2 gxr116 gxr116  6 1月  22 22:36 Documents\r\n", strlen("drwxr-xr-x. 2 gxr116 gxr116  6 1月  22 22:36 Documents\r\n"), 0);
	list_common(sess);


//4.关闭连接
	close(sess->data_fd);
	sess->data_fd = -1;

//5.回复226
	ftp_reply(sess, FTP_TRANSFEROK, "Directory send OK.");
}



//主动连接
static void do_port(session_t *sess){
	//port  192,168,109,1,7,50
	unsigned int v[6] = {0};
	sscanf(sess->arg, "%u,%u,%u,%u,%u,%u", &v[0],&v[1],&v[2],&v[3],&v[4],&v[5]);//格式化

	sess->port_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

	unsigned char *p = (unsigned char *)&sess->port_addr->sin_port;//指向端口空间
	p[0] = v[4];
	p[1] = v[5];

	p = (unsigned char *)&sess->port_addr->sin_addr;
	p[0] = v[0];
	p[1] = v[1];
	p[2] = v[2];
	p[3] = v[3];

	sess->port_addr->sin_family = AF_INET;
	ftp_reply(sess, FTP_PROTOK, "command successful. Consider using PASV.");
}



//被动连接
static void do_pasv(session_t *sess){

	/*
	char ip[16] = "192.168.109.137";//服务器的IP地址
	//被动连接的端口号是自动生成的
	sess->pasv_listen_fd = tcp_server(ip, 0);//port为0代表生成临时端口号

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(struct sockaddr);
	if(getsockname(sess->pasv_listen_fd, (struct sockaddr*)&address, &addrlen) < 0)
		ERR_EXIT("getsockname");

	unsigned short port = ntohs(address.sin_port);

	int v[4] = {0};
	sscanf(ip, "%u.%u.%u.%u", &v[0], &v[1], &v[2], &v[3]);
	char msg[MAX_BUFFER_SIZE] = {0};
	sprintf(msg, "Entering Passive Mode (%u,%u,%u,%u,%u,%u).", v[0],v[1],v[2],v[3], port>>8, port&0x0000ff);
	ftp_reply(sess, FTP_PASVOK, msg);
	*/


	char ip[16] = "192.168.109.138";
	priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_LISTEN);
	
	unsigned short port = (unsigned short)priv_sock_get_int(sess->child_fd);

	int v[4] = {0};
	sscanf(ip, "%u.%u.%u.%u", &v[0], &v[1], &v[2], &v[3]);
	char msg[MAX_BUFFER_SIZE] = {0};
	sprintf(msg, "Entering Passive Mode (%u,%u,%u,%u,%u,%u).", v[0],v[1],v[2],v[3], port>>8, port&0x0000ff);
	ftp_reply(sess, FTP_PASVOK, msg);
}