#include "common.h"
#include "sysutil.h"
#include "session.h"

#include "ftpcodes.h"
#include "ftpproto.h"
#include "tunable.h"
#include "parseconf.h"
#include "hash.h"

void ParseConf_Test( )
{
	parseconf_load_file("miniftp.conf");

	printf("tunable_pasv_enable = %d\n", tunable_pasv_enable);
	printf("tunable_port_enable = %d\n", tunable_port_enable);
	printf("tunable_listen_port = %d\n", tunable_listen_port);
	printf("tunable_max_clients = %d\n", tunable_max_clients);
	printf("tunable_max_per_ip = %d\n", tunable_max_per_ip);
	printf("tunable_accept_timeout = %d\n", tunable_accept_timeout);
	printf("tunable_connect_timeout = %d\n", tunable_connect_timeout);
	printf("tunable_idle_session_timeout = %d\n", tunable_idle_session_timeout);
	printf("tunable_data_connection_timeout = %d\n", tunable_data_connection_timeout);
	printf("tunable_loacl_umask = %d\n", tunable_local_umask);
	printf("tunable_upload_max_rate = %d\n", tunable_upload_max_rate);
	printf("tunable_download_mas_rate = %d\n", tunable_download_max_rate);
	printf("tunable_listen_address = %s\n", tunable_listen_address);
}

extern session_t *p_sess;

//最大连接数限制
static unsigned int s_children; 
void handle_sigchld(int sig);

//每一个ip连接数限制
unsigned int hash_func(unsigned int buckets, void *key);
unsigned int handle_ip_count(void *ip);
void drop_ip_count(void *ip);
static hash_t *s_ip_count_hash;
static hash_t *s_pid_ip_hash;

static void check_limit(session_t *sess);



int main (int argc ,  char *argv[ ]){

	ParseConf_Test();

    //获取当前用户的id号
	if(getuid( ) != 0){
		//当前用户不是root用户
		printf("miniftp : must be started as root.\n");
		exit(EXIT_FAILURE);
	}
	
	
	//主函数先建立控制连接
	session_t sess = {
		-1, -1, "", "", "",
		//初始化内部成员变量

		0, NULL, 0, 0, 0,
		//ftp协议状态
		
		NULL, -1, -1, 0,
		//数据连接

		-1, -1,
		//父子进程通道
		
		0, 0, 0, 0
		//限速
	};
	

	p_sess = &sess;

	sess.upload_max_rate = tunable_upload_max_rate;
	sess.download_max_rate = tunable_download_max_rate;

	//申请ip和count对应的哈希表
	s_ip_count_hash = hash_alloc(MAX_BUCKET_SIZE, hash_func);
	//申请pid和ip对应的哈希表
	s_pid_ip_hash = hash_alloc(MAX_BUCKET_SIZE, hash_func);


	//int listenfd = tcp_server("192.168.109.138",1272);
	int listenfd = tcp_server(tunable_listen_address, tunable_listen_port);
	//创建一个监听套接字

	//signal(SIGCHLD, SIG_IGN);
	signal(SIGCHLD, handle_sigchld);

	
	int sockConn;//新的套接字
	struct sockaddr_in addrCli;//客户端地址
	socklen_t addrlen;//地址长度

	while(1){
		if((sockConn = accept(listenfd,(struct sockaddr*)&addrCli,&addrlen)) < 0)
			ERR_EXIT("accept");//接受连接失败


		
		//设置最大连接数限制
		++s_children;
		sess.num_clients = s_children;

		//规定每ip连接数限制
		unsigned int ip = addrCli.sin_addr.s_addr;
		sess.num_per_ip = handle_ip_count(&ip);


		pid_t pid = fork( );//创建进程

		if(pid == -1){
			--s_children;
			ERR_EXIT("fork");//进程创建失败
		}

		if(pid == 0){
			//Child Process
			close(listenfd);//子进程把建立的描述符关掉

			//会话
			sess.ctrl_fd = sockConn;
			check_limit(&sess);//进行连接数检查限制
			begin_session(&sess);
			//相当于主进程中的子进程在创建子进程
			exit(EXIT_SUCCESS);
		}
		else{//pid>0
			//Parent Process
			//登记每个进程所对应的ip
			hash_add_entry(s_pid_ip_hash, &pid, sizeof(pid), &ip, sizeof(ip));
			close(sockConn);//父进程把新建立的套接字关掉
		}
	}
	//主进程永远不停地等待客户端连接
	close(listenfd);//主进程自己关闭所创建的套接字
	return 0;
}



static void check_limit(session_t *sess){
	if(tunable_max_clients!=0 && sess->num_clients>tunable_max_clients){
		ftp_reply(sess, FTP_TOO_MANY_USERS, "There are too many connected users, please try later");
		exit(EXIT_FAILURE);//超过连接数限制，回复421
	}
	if(tunable_max_per_ip!=0 && sess->num_per_ip>tunable_max_per_ip){
		ftp_reply(sess, FTP_IP_LIMIT, "There are too many connections from your internet address");
		exit(EXIT_FAILURE);
	}
}

void handle_sigchld(int sig){
	pid_t pid;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
		--s_children;
		unsigned int *ip = hash_lookup_entry(s_pid_ip_hash, &pid, sizeof(pid));
		if(ip == NULL)
			continue;
		drop_ip_count(ip);
		hash_free_entry(s_pid_ip_hash, &pid, sizeof(pid));
	}
}

unsigned int hash_func(unsigned int buckets, void *key){
	return (*(unsigned int*)key % buckets);
}

unsigned int handle_ip_count(void *ip){
	int count = 0;
	unsigned int *p_count = hash_lookup_entry(s_ip_count_hash, ip, sizeof(unsigned int));
	if(p_count == NULL){
		count = 1;
		hash_add_entry(s_ip_count_hash, ip, sizeof(unsigned int), &count, sizeof(unsigned int));
	}
	else{
		count = *p_count;
		++count;
		*p_count = count;
	}
	return count;
}

void drop_ip_count(void *ip){
	unsigned int *p_count = hash_lookup_entry(s_ip_count_hash, ip, sizeof(unsigned int));
	if(p_count == NULL)
		return;
	int count = *p_count;
	--count;
	*p_count = count;
	if(count == 0)
		hash_free_entry(s_ip_count_hash, ip, sizeof(unsigned int));
}