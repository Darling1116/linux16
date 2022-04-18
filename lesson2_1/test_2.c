#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//进程等待：以非阻塞的方式等待

int main(){
printf("begin start...\n");

pid_t id = fork();
//fork()之后，父子进程代码共享，数据各自私有
if(id == 0){ //child
	int count = 0;
	while(count<5){
		printf("child %d is run\n", getpid());
		sleep(1);
		count++;
	}
	exit(10);
}

else if(id > 0){ //father
	int status = 0;
	pid_t ret = 0;
	do{
		ret = waitpid(id, &status, WNOHANG);  //WNOHANG表示以非阻塞的方式等待
		if(ret==0){
			printf("child is running...\n");
		}
		sleep(2);
	}while(ret==0); //ret为0表示当前子进程还没有退出

	if(WIFEXITED(status)){  //调用系统函数判断是否是正常退出
			printf("status: %d\n", status);
			printf("child exit code: %d\n", WEXITSTATUS(status));  //提取正常退出的退出码
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
