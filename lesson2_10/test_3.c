#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

//通过硬件异常产生信号
void handler(int signo){
	printf("catch a signo: %d\n", signo);
}

int main(){
	signal(8, handler);  //SIGSEGV
	//int res = 10/0;
	sleep(2);
	int *p = NULL;
	*p = 100;

	while(1);

	return 0;
}