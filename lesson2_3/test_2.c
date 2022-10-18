	#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//C文件相关接口

int main(){

//读文件
FILE *fp = fopen("myfile.txt", "r");  

if(fp==NULL){
	printf("open file failed!\n");
	exit(1);
}

char buf[1024];
ssize_t s = fread(buf, 1, 10, fp);
if(s >0){
	buf[s] = 0;
	printf("%s", buf);
}
else{
	printf("fread error!\n");
}


fclose(fp);

return 0;
}