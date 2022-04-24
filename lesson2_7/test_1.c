#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//使用命名管道，对文件进行拷贝
int main(int argc, char *argv[]){
 mkfifo("pipe", 0644);  //建立命名管道

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








