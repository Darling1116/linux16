#include "ThreadPool.hpp"


int main(){
	ThreadPool *tp = new ThreadPool(5);
	//��һ���߳��������������������ʣ�µ��̴߳�������
	tp->ThreadPoolInit();

	while(true){
		int _b = rand()%10+1;
		Task t(_b);
		tp->Put(t);
		sleep(1);
	}

	return 0;
}