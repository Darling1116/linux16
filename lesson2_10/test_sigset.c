#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

//普通信号：1-31号

void show_pending(sigset_t *pending){
	int sig=1;
	for(; sig<=31; sig++){
		if(sigismember(pending, sig)){
			 printf("1");
		}
		else{
			printf("0");
		}
	}
	printf("\n");
}

void handler(int signo){
	printf("catch a signo: %d\n");
}


int main(){

//ctrl+C之后，2号信号产生
//此时可以使用信号捕捉，继续获取其后的pending表的状态信息
signal(2, handler);


//屏蔽2号信号集:
//一旦接收到2号信号(未决状态)，对应Pending表中的内容就会由0变为1
sigset_t block;
sigset_t oblock;
sigemptyset(&block);
sigemptyset(&oblock);
sigaddset(&block, 2);

//在设置信号屏蔽字之前，先把原来的信号屏蔽字备份到oblock中
sigprocmask(SIG_SETMASK, &block, &oblock);


//展示pending表
sigset_t pending;
int count = 0;
while(1){
	sigemptyset(&pending);
	sigpending(&pending);  //获取当前进程的未决pending信号集
	show_pending(&pending);
	sleep(1);
	count++;
	if(count == 10){
		printf("recover sig mask!\n");
		sigprocmask(SIG_SETMASK, &oblock, NULL);
	}
	
}

}