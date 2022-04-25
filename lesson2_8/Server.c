#include "common.h"

int main(){

mkfifo("./myfifo", 0644);

int fd = open("./myfifo", O_RDONLY);
if(fd < 0){
	printf("open error...\n");
	exit(1);
}

char buf[64];
while(1){
	ssize_t s = read(fd, buf, sizeof(buf)-1);
	if(s > 0){
		buf[s] = 0;
		printf("Client say# %s", buf);
	}
	else if(s == 0){
	}
	else{
		printf("read error...\n");
		break;  //当前一次读取失败
	}
}

close(fd);
return 0;
}