#include <stdio.h>
#include <signal.h>


volatile int flag = 0;
//int flag = 0;

void handler(int sig){
	flag = 1;
	printf("chage flag 0 to 1\n");
}

//while 循环检查的flag，并不是内存中最新的flag
//这就存在了数据二异性的问题
//编译时加上了-o2选项，while检测的flag其实已经因为优化，被放在了CPU寄存器当中


int main(){
 signal(2, handler);
 while(!flag);
 printf("process quit normal\n");

 return 0;
}



