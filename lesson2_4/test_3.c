#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
//�ض���

close(1);  //�رձ�׼���stdout�ļ�������
int fd = open("myfile", O_WRONLY | O_CREAT);  
if(fd < 0){
	printf("open file error!\n");
	exit(1);
}
printf("fd: %d", fd);  //����Ӧ�����뵽��ʾ���ϵĶ��������ض������ļ���
fflush(stdout);

close(fd);

return 0;
}