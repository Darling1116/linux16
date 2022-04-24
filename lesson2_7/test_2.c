#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 

int main(){
 int infd;
 infd = open("pipe", O_RDONLY);
 int outfd = open("def.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
 char buf[64];
 int n;
 while ((n=read(infd, buf, 64))>0){
	write(outfd, buf, n);
 }
 close(infd);
 close(outfd);
 return 0;
}
