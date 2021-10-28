#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void *route_1(void* arg){
    char *name = (char*)arg;
    while(1){
        pthread_cond_wait(&cond, &lock);   
        printf("%s get cond...\n", name);
    }
}

void *route_2(void* arg){
    char *name = (char*)arg;
    while(1){
        usleep(1000);
        pthread_cond_signal(&cond);
        printf("%s signal done...\n", name);
    }
}



int main(){
    pthread_t tid1, tid2;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_create(&tid1, NULL, route_1, "thread 1");
    pthread_create(&tid2, NULL, route_2, "thread 2");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);

    return 0;

}
