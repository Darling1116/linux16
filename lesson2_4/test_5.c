#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

 int fd = open("myfile", O_CREAT | O_RDWR);
 if (fd < 0) {
	perror("open");
	exit(1);
 }

 close(1);  //¹Ø±Õstdout

 dup2(fd, 1);  //oldfd,newfd: newfd is the copy of oldfd.

 for (;;) {
	char buf[1024] = {0};
	ssize_t read_size = read(0, buf, sizeof(buf) - 1);
	if (read_size < 0) {
		perror("read");
		break;
	}
	printf("%s", buf);
	fflush(stdout);
 }

	close(fd);
	return 0;
}

