#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void *route_1(void *arg){
    char *name = (char*)arg;
    while(1){
        usleep(1000);
        pthread_cond_signal(&cond);
        printf("%s is pushing signal...\n", name);
    }
}

void *route_2(void *arg){
    char *name = (char*)arg;
    while(1){
        pthread_cond_wait(&cond, &lock);
        printf("%s is waiting...\n", name);
    }
}


int main(){
    pthread_t tid1,tid2,tid3,tid4,tid5;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, &lock);
    pthread_create(&tid1, NULL, route_1, (void*)"thread 1");
    pthread_create(&tid2, NULL, route_2, (void*)"thread 2");
    pthread_create(&tid3, NULL, route_2, (void*)"thread 3");
    pthread_create(&tid4, NULL, route_2, (void*)"thread 4");
    pthread_create(&tid5, NULL, route_2, (void*)"thread 5");


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;

}
