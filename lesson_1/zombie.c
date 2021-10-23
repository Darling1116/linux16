#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
  printf("bebin start....\n");
  sleep(5);
  pid_t id = fork();
  if(id == 0){
	printf("child[%d] is begin z...\n", getpid());
	sleep(5);
	exit(1);
  }
  else if(id > 0){
	printf("father[%d] is sleeping...\n", getpid());
	sleep(30);
	exit(2);
  }
  else{
	printf("error\n");
  }
  return 0;
}
