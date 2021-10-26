#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void *arg){
  int i=1;
  int sum=0;
  for(; i<=100; i++){
	sum += i;
  }
  return (void *)sum;
  //pthread_detach(pthread_self());
}


int main(){
  pthread_t tid;
  pthread_create(&tid, NULL, thread_run, (void *)"thread 4");
  //pthread_detach(tid);
  sleep(5);
  void *code;
  int ret =  pthread_join(tid, &code);
  printf("sum is %lu, ret=%d\n", (long long)code, ret);
  return 0;
}
