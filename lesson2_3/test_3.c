#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//C文件相关接口

int main(){

//输出信息到显示器的三种方法

printf("1: hello printf...\n");

const char* msg = "2: hello pwrite...\n";
fwrite(msg, strlen(msg), 1, stdout);

fprintf(stdout, "3: hello fprintf...\n");
return 0;

}