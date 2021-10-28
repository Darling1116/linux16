#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 1000;
pthread_mutex_t lock;

void *get_ticket(void *arg){
    int i = (int)arg;
    while(1){
        pthread_mutex_lock(&lock);
        if(ticket>0){
            usleep(1000);
            printf("thread %d get num %d ticket\n", i, ticket);
            ticket--;
            pthread_mutex_unlock(&lock);
        }
        else{
            pthread_mutex_unlock(&lock);
            break;
        }
    }
}


int main(){
    printf("main thread start...\n");

    pthread_t tid[4];
    int i=0;
    
    pthread_mutex_init(&lock, NULL);

    for(i=0; i<4; i++){
        pthread_create(tid+i, NULL, get_ticket, (void*)i);
    }

    for(i=0; i<4; i++){
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}

