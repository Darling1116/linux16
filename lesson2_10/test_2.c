#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

//ͨ��������������ź�

int main(){

int count = 0;
alarm(5);
while(1){
	printf("I am running...count: %d\n", count++);
	sleep(1);

}
}