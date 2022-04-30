#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

void handler(int signo){
	printf("I get a signo: %d\n", signo);
}

int main(){

struct sigaction act, oact;

act.sa_handler = handler;
act.sa_flags = 0;
sigemptyset(&act.sa_mask);

sigaction(2, &act, &oact);

while(1);

}