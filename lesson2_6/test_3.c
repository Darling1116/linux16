#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
 mkfifo("pipe", 0644);

 int fd = open("pipe", O_WRONLY);
 int infd = open("abc.txt", O_RDONLY); 
 char buf[64];
 int n;
 while ((n=read(infd, buf, 64))>0){
 write(fd, buf, n);
 }
 close(infd);
 close(fd);
 return 0;
}