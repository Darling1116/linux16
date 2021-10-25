#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void* arg){
  while(1){
	printf("new thread...%s,pid: %d, tid: %lu \n",(char*)arg,  getpid(), pthread_self());
	sleep(2);
  }
}

int main(){
  pthread_t tid;
  pthread_create(&tid, NULL, thread_run, (void*)"thread 1");
  printf("tid: %lu\n", tid);  //unsigned long int
  while(1){
	printf("main thread...pid: %d, tid: %lu\n", getpid(), pthread_self());
	sleep(1);
  }
  return 0;
}
