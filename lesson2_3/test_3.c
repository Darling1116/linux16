#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//C�ļ���ؽӿ�

int main(){

//�����Ϣ����ʾ�������ַ���

printf("1: hello printf...\n");

const char* msg = "2: hello pwrite...\n";
fwrite(msg, strlen(msg), 1, stdout);

fprintf(stdout, "3: hello fprintf...\n");
return 0;

}