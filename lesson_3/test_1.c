#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void *arg){
  char *msg = (char *)arg;
  while(1){
	printf("new thread...,%s, %d, %d\n",msg, getpid(), getppid());
	sleep(2);
  }
}



int main(){
  pthread_t tid1;
  pthread_t tid2;
  pthread_t tid3;
  pthread_t tid4;
  pthread_t tid5;
  pthread_t tid6;
  pthread_create(&tid1, NULL, thread_run, (void *)"thread one");
  pthread_create(&tid2, NULL, thread_run, (void *)"thread two");
  pthread_create(&tid3, NULL, thread_run, (void *)"thread three");
  pthread_create(&tid4, NULL, thread_run, (void *)"thread four");
  pthread_create(&tid5, NULL, thread_run, (void *)"thread five");
  pthread_create(&tid6, NULL, thread_run, (void *)"thread six");
  
  //main thread
  while(1){
	printf("main thread...,%d, %d\n", getpid(), getppid());
	sleep(1);
  }
  return 0;
}
