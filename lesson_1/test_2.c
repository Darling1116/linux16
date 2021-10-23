#include <stdio.h>
#include <stdlib.h>

int main(){
  printf("begin start.....\n");
  pid_t id = fork();
  if(id == 0){
	printf("child[%d] is alone...\n", getpid());
	sleep(20);
  }
  else if(id > 0){
	printf("father[%d] is exit...\n", getpid());
	sleep(3);
	exit(1);
  }
  else{
	printf("error.....\n");
  }
}
