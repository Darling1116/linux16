#include "ftpproto.h"
#include "ftpcodes.h"
#include "str.h"

void ftp_reply(session_t *sess, int code, const char *text){
	char buf[MAX_BUFFER_SIZE] = {0};
	sprintf(buf, "%d %s\r\n", code, text);
	send(sess->ctrl_fd, buf, strlen(buf), 0);
}



////////����ӳ�����:������ӳ�䵽ָ��������ȥִ��/////////

static void do_user(session_t *sess);
static void do_pass(session_t *sess);
static void do_syst(session_t *sess);//��ȡϵͳ��Ϣ
static void do_feat(session_t *sess);//��ȡ����������


typedef struct ftpcmd{
	const char *cmd;
	void(*cmd_handler)(session_t *sess);
}ftpcmd_t;

//����ӳ���
static ftpcmd_t ctrl_cmds[] = {
	{"USER", do_user},
	{"PASS", do_pass},
	{"SYST", do_syst},
	{"FEAT", do_feat}
};
//��������ͺ���




//ftp�������
void handle_child(session_t *sess){
	//send(sess->ctrl_fd,"220 (miniftp 3.0.2)\n\r",strlen("220 (miniftp 3.0.2)\n\r"),0);
	//220����׼������
	ftp_reply(sess, FTP_GREET, "(myminiftp 1.0)");
	
	int ret;
	while(1){
		//��ͣ�صȴ��ͻ��˵�������д�����������
		memset(sess->cmdline, 0, MAX_COMMAND_LINE);
		memset(sess->cmd, 0, MAX_COMMAND);
		memset(sess->arg, 0, MAX_ARG);
		ret = recv(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE, 0);
		if(ret == -1)
			ERR_EXIT("readline");
		else if (ret == 0)
			exit(EXIT_SUCCESS);
		else 
			str_trim_crlf(sess->cmdline);
			printf("cmdline = %s\n", sess->cmdline);
		//��������---���ݵ���ɣ�����+�ո�+����
			str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
		//�ָ�������
			printf("cmd = %s\n",sess->cmd);
			printf("arg = %s\n",sess->arg);
		

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


//����û����Ƿ�ƥ��
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


//��Ȩ:�û��ڴ��ڵ�����£������û�
//PASS 123456
static void do_pass(session_t *sess){
	struct passwd *pwd = getpwuid(sess->uid);
	if(pwd == NULL){
		ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
		return;
	}

	struct spwd *spd = getspnam(pwd->pw_name);
	//�����û�����ȡ�û���Ӱ���ļ�
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

	setegid(pwd->pw_gid);
	seteuid(pwd->pw_uid);
	chdir(pwd->pw_dir);
	
	ftp_reply(sess, FTP_LOGINOK, "Login successful.");
}

//��ȡϵͳ��Ϣ
static void do_syst(session_t *sess){
	ftp_reply(sess, FTP_SYSTOK, "UNIX Type: L8");
}

//��ȡ����������
static void do_feat(session_t *sess){
	ftp_reply(sess, FTP_BADCMD, "FEAT GET");
}
