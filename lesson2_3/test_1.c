#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//C�ļ���ؽӿ�

int main(){

//д�ļ�
//FILE *fp = fopen("myfile.txt", "w");  //��w�ķ�ʽ�򿪣��������һ���ļ��������
FILE *fp = fopen("myfile.txt", "a");  ////��׷�ӵķ�ʽ��

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