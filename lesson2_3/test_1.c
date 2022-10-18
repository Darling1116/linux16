#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//C文件相关接口

int main(){

//写文件
//FILE *fp = fopen("myfile.txt", "w");  //以w的方式打开，会清空上一次文件里的内容
FILE *fp = fopen("myfile.txt", "a");  ////以追加的方式打开

if(fp==NULL){
	printf("open file failed!\n");
	exit(1);
}

const char* msg = "hello world.\n";
int count = 5;
while(count!=0){
	//fputc(fp);
	fwrite(msg, strlen(msg), 1, fp);
	count--;
}


fclose(fp);

return 0;
}