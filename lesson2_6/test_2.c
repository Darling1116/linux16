#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//匿名管道

int main(){
int fd[2] = {0};  //定义一个文件描述符数组:fd[0]--read,fd[1]--write
int pi = pipe(fd);  //创建匿名管道
if(pi != 0){
	printf("pipe erroe!\n");
	exit(1);
}

pid_t id = fork();
if(id<0){
	printf("fork error!\n");
	exit(2);
}
else if(id==0){
	//child:write
	close(fd[0]);

	const char* msg = "I am a great girl...\n";
	int count = 0;
	while(1){
		write(fd[1], msg, strlen(msg));
		//sleep(3);
		count++;
		/*
		if(count==10){
			close(fd[1]);
			break;
		}
		*/
	}
}
else{
	//father:read
	close(fd[1]);
	
	int count = 0;
	char buf[64];
	while(1){
		ssize_t s = read(fd[0], buf ,10);   //s是读取的长度
		if(s > 0){
			count++;
			printf("s is %d\n", s);
			buf[s] = 0;
			printf("father get message: %s\n", buf);
			if(count==20){
				close(fd[0]);
				break;
			}
			sleep(3);
		}
	}
	return 0;
}


}