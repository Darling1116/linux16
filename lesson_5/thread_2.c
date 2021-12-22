#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 1000;

void *get_ticket(void *arg){
    char *msg = (char *)arg;
    while(1){
        if(ticket>0){
            usleep(1000);
            printf("%s: get ticket nunber: %d\n", msg, ticket);
            ticket--;
        }
        else{
            break;
        }
    }
}


int main (){
    printf("main start...\n");

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
 
    pthread_create(&tid1, NULL, get_ticket, (void *)"thread 1");
    pthread_create(&tid2, NULL, get_ticket, (void *)"thread 2");
    pthread_create(&tid3, NULL, get_ticket, (void *)"thread 3");
    pthread_create(&tid4, NULL, get_ticket, (void *)"thread 4");
    //sleep(5);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    return 0;

}

