#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <sys/shm.h>

//ͨ��ϵͳ��������̷��ź�
void handler(int signo){
	printf("catch a signal: %d\n", signo);
}


int main(int argc, char* argv[]){
	//printf("My pid id %d", getpid());
	//signal(2, handler);

	if(argc==3){
		kill(atoi(argv[1]), atoi(argv[2]));
	}
	/*

	int count = 5;
	while(1){
		printf("I am running...\n");
		sleep(1);
	}
	*/

	// raise(2);  //���Լ����ź�
	// abort();

}