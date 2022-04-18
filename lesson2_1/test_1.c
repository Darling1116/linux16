#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//进程等待

int main(){
printf("begin start...\n");

pid_t id = fork();
//fork()之后，父子进程代码共享，数据各自私有
if(id == 0){ //child
	int count = 0;
	while(count<5){
		printf("child %d is running...\n", getpid());
		sleep(1);
		count++;
	}
	exit(10);
}
else if(id > 0){ //father
	int status = 0;
	//pid_t ret = wait(&status);
	pid_t ret = waitpid(id, &status, 0);  //0表示以阻塞的方式等待
	/*
	if(ret>0 && (status&0x7f)==0){  //正常退出
		printf("status: %d\n", status);
		printf("child exit code: %d\n", (status>>8)&0xff);
		printf("father wait end...\n");
	}
	*/
	if(WIFEXITED(status)){  //调用系统函数判断是否是正常退出
		printf("ret: %d\n", ret);
		printf("child exit code: %d\n", WEXITSTATUS(status));  //提取正常退出的退出码
		printf("child exit code: %d\n", (status>>8)&0xff);
		printf("father wait end...\n");
	}
	else if(ret>0){
		printf("status: %d\n", status);
		printf("child exit code: %d\n", (status)&0x7f);
		printf("father wait end...\n");
	}
	else{
		printf("wait failed...\n");
	}
}
else{ //error
	printf("error......");
}
}

