#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void *arg){
  printf("new thread...:%s:\n", (char *)arg);
  int i=1;
  int sum=0;
  for(i=1; i<=100; i++){
	sum += i;
  }
  return (void*)sum;
}


int main (){
  pthread_t tid;
  pthread_create(&tid, NULL, thread_run, (void *)"thread 1");
  printf("main thread...\n");
  void *code;
  pthread_join(tid, &code);
  printf("sum is %lu\n", (long long)code);
  sleep(5);
  return 0;
}
