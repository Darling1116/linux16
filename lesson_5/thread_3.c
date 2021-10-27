#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 1000;
pthread_mutex_t lock;

void *get_ticket(void *arg){
    int num = (int)arg;
    while(1){
        pthread_mutex_lock(&lock);
        if(ticket>0){
            usleep(1000);
            printf("thread %d get ticket: num %d\n", num, ticket);
            ticket--;
            pthread_mutex_unlock(&lock);
        }
        else{
            pthread_mutex_unlock(&lock);
            break;
        }
    }
   // pthread_exit(1);

}


int main(){
    pthread_t tid[4];
    int i=0;
    pthread_mutex_init(&lock, NULL);
    for(;i<4;i++){
        pthread_create(tid+i, NULL, get_ticket, (void*)i);
    }
    
    //sleep(5);
    for(i=0;i<4;i++){
       int ret= pthread_join(tid[i], NULL);
       printf("tid %d, ret: %d\n", i, ret);
    }
    pthread_mutex_destroy(&lock);
    //sleep(10);
    return 0;

}

