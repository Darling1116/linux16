#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//���̳����滻
int main(){
char* const argv[] = {"ps", "-ef", NULL};

//execv("/bin/ps", argv);
//execvp("ps", argv);
//execl("/bin/ps", "ps", "-e", "-f", NULL);
execl("./mycmd", "./mycmd", NULL);

printf("hahahhahaha...\n");  //�滻������Ĵ��벻�ᱻִ��

return 0;
}