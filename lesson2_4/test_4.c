#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
 //库函数的缓冲区是二次加上的，由C标准库提供
 close(1);
 int fd = open("myfile", O_WRONLY | O_CREAT);  
 const char *msg0="hello printf\n";
 const char *msg1="hello fwrite\n";
 const char *msg2="hello write\n";

 printf("%s", msg0);
 fwrite(msg1, strlen(msg0), 1, stdout);
 write(1, msg2, strlen(msg2));

 fork();
 return 0;



return 0;

}