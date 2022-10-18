#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//实现一个简单的minishell

#define SIZE 64
#define NUM 16

int main(){
	char cmd[SIZE];
	const char* cmd_line = "[gxr116@Centos_7 lesson2_1]$ ";
	while(1){
		cmd[0] = 0;
		printf("%s", cmd_line);
		fgets(cmd, SIZE, stdin);  //命令行参数通过标准输入写入cmd
		printf("%s", cmd);
		cmd[strlen(cmd)-1] = '\0';  //把换行符替换为结束标志\0

		char* argv[NUM];  //定义一个字符串数组，用来存放解析的结果
		argv[0] = strtok(cmd, " ");  //以空格来分割字符串，把第一个子串放在argv[0]中
		int i= 1;
		do{
			argv[i] = strtok(NULL, " ");  //分割剩下的字符串
			if(argv[i] == NULL){
				break;
			}
			i++;
		}while(1);

		
		//测试命令行参数是否能被正确解析
		/*
		int j = 0;
		for( ; j<i; j++){
			printf("argv[%d]: %s\n",j, argv[j]);
		}
		*/

		pid_t id = fork();
		if(id == 0){
			//child：命令行解析
			execvp(argv[0], argv);  //执行进程程序替换，替换成功，则不再返回
			exit(1); //替换失败，则程序退出
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