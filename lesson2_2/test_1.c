#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//进程程序替换
int main(){
char* const argv[] = {"ps", "-ef", NULL};

//execv("/bin/ps", argv);
//execvp("ps", argv);
//execl("/bin/ps", "ps", "-e", "-f", NULL);
execl("./mycmd", "./mycmd", NULL);

printf("hahahhahaha...\n");  //替换函数后的代码不会被执行

return 0;
}