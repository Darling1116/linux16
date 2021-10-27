#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 100;

void *thread(void *arg){
    char *msg = (char *)arg;
    printf("%s... count=%d\n", msg, count);
    pthread_exit(NULL);
}



int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, thread, (void *)"new thread");
    sleep(5);
    printf("main thread...count=%d\n", count);
    int ret = pthread_join(tid, NULL);
    printf("ret: %d\n", ret);
    return 0;
}

