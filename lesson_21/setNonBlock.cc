#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

void setNonBlock(int fd){
	int fl = fcntl(fd, F_GETFL);  //����ļ���������״̬���
	if (fl < 0){
		cout << "fcntl errpr!\n" << endl;
		return;
	}

	fcntl(fd, F_SETFL, fl | O_NONBLOCK);  //�Ѹ��ļ���������IO����Ϊ������ʽ
}


int main(){
	char c = 0;
	setNonBlock(0);  
	//��Ϊ����ʽ�����һֱ�ȴ��û����룻��Ϊ������ʽ֮������ȡʧ�ܣ������������Ϣ
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



