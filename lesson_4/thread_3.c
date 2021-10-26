#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void *arg){
  int i=1;
  int sum=0;
  printf("new thread...%s\n", (char *)arg);
  for(; i<=100; i++){
	sum += i;
  }
  return (void *)sum;
  //pthread_cancel(pthread_self());
}

int main(){
  pthread_t tid;
  pthread_create(&tid, NULL, thread_run, (void *)"thread_3");
  sleep(5);
  pthread_cancel(tid);

  printf("cancel new thread done..\n");
  printf("main thread...\n");
  return 0;
}
