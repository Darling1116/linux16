#include "ThreadPool.hpp"


int main(){
	ThreadPool *tp = new ThreadPool(5);
	//��һ���߳��������������������ʣ�µ��̴߳�������
	tp->ThreadPoolInit();

	//server
	int count = 20;
	while(true){
		int _b = rand()%10+1;
		Task t(_b);
		tp->Put(t);
		sleep(1);
		count--;
	}
	//tp->ThreadQuit();
	return 0;
}