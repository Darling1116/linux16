#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

bool setNonBlock(int fd){
	int fl = fcntl(fd, F_GETFL);  //����ļ���������״̬���
	if (fl < 0){
		cerr << "fcntl error!\n" << endl;
		return false;
	}
	fcntl(fd, F_SETFL, fl | O_NONBLOCK);  //�Ѹ��ļ���������IO��ʽ����Ϊ������ʽ
	return true;
}


int main(){
	setNonBlock(0);  //����0���ļ�������Ϊ������IO
	//��Ϊ����ʽ�����һֱ�ȴ��û����룻��Ϊ������ʽ֮������ȡʧ�ܣ������������Ϣ
	while(1){  
		char buffer[1024] = {0};
		ssize_t s = read(0, buffer, sizeof(buffer)-1);  //�ӱ�׼�����ж�ȡ��Ϣ
		if(s < 0){
			if(errno==EAGAIN || errno==EWOULDBLOCK){
				cout << "try again......" << endl;
				sleep(1);  ////��ѯ���û���Ʒ���ģ�OSִ��
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



