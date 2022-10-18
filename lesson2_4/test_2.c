#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//系统文件IO

int main(){

//读操作
int fd = open("myfile", O_RDONLY);
if(fd < 0){
	printf("open file error!\n");
	exit(1);
}

char buf[1024];
ssize_t s = read(fd, buf, 6);
if(s < 0){
	printf("read error!\n");
	exit(1);
}

buf[s] = 0;  //
printf("%s", buf);

close(fd);
return 0;
}

