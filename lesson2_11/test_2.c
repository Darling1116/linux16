#include <stdio.h>
#include <signal.h>


volatile int flag = 0;
//int flag = 0;

void handler(int sig){
	flag = 1;
	printf("chage flag 0 to 1\n");
}

//while ѭ������flag���������ڴ������µ�flag
//��ʹ��������ݶ����Ե�����
//����ʱ������-o2ѡ�while����flag��ʵ�Ѿ���Ϊ�Ż�����������CPU�Ĵ�������


int main(){
 signal(2, handler);
 while(!flag);
 printf("process quit normal\n");

 return 0;
}



