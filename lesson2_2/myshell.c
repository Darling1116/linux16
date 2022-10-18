#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//ʵ��һ���򵥵�minishell

#define SIZE 64
#define NUM 16

int main(){
	char cmd[SIZE];
	const char* cmd_line = "[gxr116@Centos_7 lesson2_1]$ ";
	while(1){
		cmd[0] = 0;
		printf("%s", cmd_line);
		fgets(cmd, SIZE, stdin);  //�����в���ͨ����׼����д��cmd
		printf("%s", cmd);
		cmd[strlen(cmd)-1] = '\0';  //�ѻ��з��滻Ϊ������־\0

		char* argv[NUM];  //����һ���ַ������飬������Ž����Ľ��
		argv[0] = strtok(cmd, " ");  //�Կո����ָ��ַ������ѵ�һ���Ӵ�����argv[0]��
		int i= 1;
		do{
			argv[i] = strtok(NULL, " ");  //�ָ�ʣ�µ��ַ���
			if(argv[i] == NULL){
				break;
			}
			i++;
		}while(1);

		
		//���������в����Ƿ��ܱ���ȷ����
		/*
		int j = 0;
		for( ; j<i; j++){
			printf("argv[%d]: %s\n",j, argv[j]);
		}
		*/

		pid_t id = fork();
		if(id == 0){
			//child�������н���
			execvp(argv[0], argv);  //ִ�н��̳����滻���滻�ɹ������ٷ���
			exit(1); //�滻ʧ�ܣ�������˳�
		}
		else if(id > 0){
			//father
			int status = 0;
			pid_t ret = waitpid(id, &status, 0);
			if(ret > 0){
				printf("status code: %d\n", (status>>8)&0xff);
			}
		}
		else{
			perror("fork error!\n");
		}

		return 0;
	}



}