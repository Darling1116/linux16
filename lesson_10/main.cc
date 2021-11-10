#include "RingQueue.hpp"

void *productor(void *arg){
	RingQueue *pq = (RingQueue*)arg;
	//int num = 10;
	while(true){
		sleep(1);
		int m = rand()%10+1;
		int n = rand()%100+1;
		Task t(m, n);
		pq->Put(t);
		cout << "p_task Task is: " << m << "*" << n << "= " << "?" << endl;
		/*
		pq->Put(num);
        num++;
		if(num > 10){
			num %= 10 +1;
		}
		std::cout << "productor done......" << std::endl;
		*/
	}
}


void *consumer(void *arg){
	RingQueue *cq = (RingQueue*)arg;
	//int data;
	Task t;
	while(true){
		//sleep(2);
		cq->Get(t);
		cout << "c_task Task is:" << t.m << "*" << t.n << "=" << t.Run() << endl;
		//int data;
		//cq->Get(data);
		//std::cout << "consumer done...# " << data << std::endl;
	}
}


int main(){
	RingQueue *rq = new RingQueue(10);

	pthread_t p;
	pthread_t c;

	pthread_create(&p, NULL, productor,(void*)rq);
	pthread_create(&c, NULL, consumer, (void*)rq);

	pthread_join(p, NULL);
	pthread_join(c, NULL);
	delete rq;
	return 0; 
}


