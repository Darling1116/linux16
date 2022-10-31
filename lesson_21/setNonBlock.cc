#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

bool setNonBlock(int fd){
	int fl = fcntl(fd, F_GETFL);  //获得文件描述符的状态标记
	if (fl < 0){
		cerr << "fcntl error!\n" << endl;
		return false;
	}
	fcntl(fd, F_SETFL, fl | O_NONBLOCK);  //把该文件描述符的IO方式设置为非阻塞式
	return true;
}


int main(){
	setNonBlock(0);  //设置0号文件描述符为非阻塞IO
	//若为阻塞式，则会一直等待用户输入；改为非阻塞式之后，若读取失败，则会输出相关信息
	while(1){  
		char buffer[1024] = {0};
		ssize_t s = read(0, buffer, sizeof(buffer)-1);  //从标准输入中读取信息
		if(s < 0){
			if(errno==EAGAIN || errno==EWOULDBLOCK){
				cout << "try again......" << endl;
				sleep(1);  ////轮询是用户设计发起的，OS执行
			}
			else if(errno==EINTR){
				cout << "data unknow..." << endl;
			}
			else{
				cout << "read error" << endl;
			}
			continue;
		}
		else{
			buffer[s] = 0;
			cout << "echo# " << buffer << endl;
		}
	}
	cout << "........................" << endl;
}



