#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
printf("begin start...");
//sleep(1);
pid_t id = fork();
if(id == 0){ //child
	//while(1){
		printf("i am child, pid:%d, ppid:%d", getpid(), getppid());
		sleep(1);
	//}
}
else if(id > 0){ //father
//	while(1){
		printf("i am father, pid:%d, ppid:%d", getpid(), getppid());
		sleep(2);
//	}
}
else{ //error
	
}



	//printf("pid:%d\n", getpid());
	//printf("ppid:%d\n", getppid());
	//printf("begin start.....");
	//sleep(3);
	//fork(); 
	//while(1){
	//	printf("I am a process, pid: %d, ppid: %d\n", getpid(), getppid());
	//	sleep(1);
	//}
	//return 0;
}
