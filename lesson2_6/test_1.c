#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//�����ܵ�
//ʵ�֣��Ӽ��̶�ȡ���ݣ�д��ܵ�����ȡ�ܵ���д����Ļ
int main(){

int fd[2] = {0};  //����һ���ļ�����������:fd[0]--read,fd[1]--write
pipe(fd);  //���������ܵ�

pid_t id = fork();
if(id==0){
	//child:write
	close(fd[0]);

	char* msg;
	while(1){
		//�Ӽ��̶�������
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
		ssize_t s = read(fd[0], buf ,sizeof(buf)-1);   //s�Ƕ�ȡ�ĳ���
		if(s > 0){
			buf[s] = 0;  //!!!
			printf("father get message: %s\n", buf);
		}
	}
	return 0;
}


}