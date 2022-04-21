#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
//重定向

close(1);  //关闭标准输出stdout文件描述符
int fd = open("myfile", O_WRONLY | O_CREAT);  
if(fd < 0){
	printf("open file error!\n");
	exit(1);
}
printf("fd: %d", fd);  //本来应该输入到显示器上的东西，被重定向到了文件中
fflush(stdout);

close(fd);

return 0;
}