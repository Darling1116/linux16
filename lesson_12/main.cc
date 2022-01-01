#include "ThreadPool.hpp"


int main(){
	ThreadPool *tp = new ThreadPool(5);
	//有一个线程往任务队列里面塞任务，剩下的线程处理任务
	tp->ThreadPoolInit();

	//server
	int count = 20;
	while(true){
		int b = rand()%10+1;
		Task t(b);
		tp->Put(t);
		sleep(1);
		count--;
	}
	tp->ThreadQuit();
	return 0;
}