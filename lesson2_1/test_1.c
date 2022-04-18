#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//���̵ȴ�

int main(){
printf("begin start...\n");

pid_t id = fork();
//fork()֮�󣬸��ӽ��̴��빲�����ݸ���˽��
if(id == 0){ //child
	int count = 0;
	while(count<5){
		printf("child %d is running...\n", getpid());
		sleep(1);
		count++;
	}
	exit(10);
}
else if(id > 0){ //father
	int status = 0;
	//pid_t ret = wait(&status);
	pid_t ret = waitpid(id, &status, 0);  //0��ʾ�������ķ�ʽ�ȴ�
	/*
	if(ret>0 && (status&0x7f)==0){  //�����˳�
		printf("status: %d\n", status);
		printf("child exit code: %d\n", (status>>8)&0xff);
		printf("father wait end...\n");
	}
	*/
	if(WIFEXITED(status)){  //����ϵͳ�����ж��Ƿ��������˳�
		printf("ret: %d\n", ret);
		printf("child exit code: %d\n", WEXITSTATUS(status));  //��ȡ�����˳����˳���
		printf("child exit code: %d\n", (status>>8)&0xff);
		printf("father wait end...\n");
	}
	else if(ret>0){
		printf("status: %d\n", status);
		printf("child exit code: %d\n", (status)&0x7f);
		printf("father wait end...\n");
	}
	else{
		printf("wait failed...\n");
	}
}
else{ //error
	printf("error......");
}
}

