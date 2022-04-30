#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

//通过软件条件产生信号

int main(){

int count = 0;
alarm(5);
while(1){
	printf("I am running...count: %d\n", count++);
	sleep(1);

}
}