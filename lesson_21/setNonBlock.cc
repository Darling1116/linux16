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

	fcntl(fd, F_SETFL, fl | O_NONBLOCK);  //把该文件描述符的IO设置为非阻塞式
}


int main(){
	char c = 0;
	setNonBlock(0);  
	//若为阻塞式，则会一直等待用户输入；改为非阻塞式之后，若读取失败，则会输出相关信息
	while(1){
		sleep(10);
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



