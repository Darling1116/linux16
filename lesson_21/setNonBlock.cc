#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

void setNonBlock(int fd){
	int fl = fcntl(fd, F_GETFL);  //获得文件描述符的状态标记
	if (fl < 0){
		cout << "fcntl errpr!\n" << endl;
		return;
	}

	fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}


int main(){
	char c = 0;
	setNonBlock(0);
	while(1){
		sleep(1);
		ssize_t s = read(0, &c, 1);
		if(s > 0){
			cout << c << endl;
		}
		else{
			cout << "errno:" << errno << endl;
			cout << "ret = " << s << endl;
		}
	}

	cout << "........................" << endl;
}



