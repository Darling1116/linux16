#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//匿名管道
//实现：从键盘读取数据，写入管道，读取管道，写到屏幕
int main(){

int fd[2] = {0};  //定义一个文件描述符数组:fd[0]--read,fd[1]--write
pipe(fd);  //创建匿名管道

pid_t id = fork();
if(id==0){
	//child:write
	close(fd[0]);

	char* msg;
	while(1){
		//从键盘读入数据
		fgets(msg, sizeof(msg), stdin);
		write(fd[1], msg, strlen(msg));
		sleep(1);
	}
}

else{
	//father:read
	close(fd[1]);
	
	int count = 0;
	char buf[64];
	while(1){
		ssize_t s = read(fd[0], buf ,sizeof(buf)-1);   //s是读取的长度
		if(s > 0){
			buf[s] = 0;  //!!!
			printf("father get message: %s\n", buf);
		}
	}
	return 0;
}


}