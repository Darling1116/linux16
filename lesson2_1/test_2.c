#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//���̵ȴ����Է������ķ�ʽ�ȴ�

int main(){
printf("begin start...\n");

pid_t id = fork();
//fork()֮�󣬸��ӽ��̴��빲�����ݸ���˽��
if(id == 0){ //child
	int count = 0;
	while(count<5){
		printf("child %d is run\n", getpid());
		sleep(1);
		count++;
	}
	exit(10);
}

else if(id > 0){ //father
	int status = 0;
	pid_t ret = 0;
	do{
		ret = waitpid(id, &status, WNOHANG);  //WNOHANG��ʾ�Է������ķ�ʽ�ȴ�
		if(ret==0){
			printf("child is running...\n");
		}
		sleep(2);
	}while(ret==0); //retΪ0��ʾ��ǰ�ӽ��̻�û���˳�

	if(WIFEXITED(status)){  //����ϵͳ�����ж��Ƿ��������˳�
			printf("status: %d\n", status);
			printf("child exit code: %d\n", WEXITSTATUS(status));  //��ȡ�����˳����˳���
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
