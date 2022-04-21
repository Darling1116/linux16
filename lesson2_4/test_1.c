#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//系统文件IO

int main(){

//写操作
int fd = open("myfile", O_WRONLY | O_APPEND | O_CREAT);  
if(fd < 0){
	printf("open file error!\n");
	exit(1);
}
printf("fd: %d", fd);

const char* msg = "i am a college girl\n";
write(fd, msg, strlen(msg));
fflush(stdin);
close(fd);

return 0;
}
