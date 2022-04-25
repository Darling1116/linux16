#include "common.h"

int main(){

int fd = open("./myfifo", O_WRONLY);
if(fd < 0){
	printf("open error...\n");
	exit(1);
}


char msg[64];
//const char* buf = "Please Enter Message # ";
while(1){
	//write(1, buf, strlen(buf));
	printf("Please Enter Message # ");  //使用printf，要注意刷新标准输出
	fflush(stdout);

	ssize_t s = read(0, msg, sizeof(msg)-1);  //从键盘读数据
	if(s > 0){
		msg[s] = 0;
		write(fd, msg, s); 
	}
	else if(s == 0){
	}
	else{
		printf("write error...\n");
		break;  
	}
}

close(fd);
return 0;
}