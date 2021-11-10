#include "RingQueue.hpp"


void *productor(void *arg){
	RingQueue *pq = (RingQueue*)arg;
	//������������100-110֮�������
	int number = 100;
	while(true){
		pq->Put(number);
		number++;
		if(number>=110){
			number = 100;
		}
		std::cout << "productor done..." << std::endl;
	}
}


void *consumer(void *arg){
	RingQueue *cq = (RingQueue*)arg;
	//sleep(1);
	int data;
	while(true){
		sleep(2);
		cq->Get(data);
		std::cout << "consumer done...# " << data << std::endl;
	}
}


int main(){
	RingQueue *rq = new RingQueue(10);

	pthread_t p;
	pthread_t c;
	//�����߳�ͨ��RingQueue���н�������
	pthread_create(&p, NULL, productor, (void*)rq);
	pthread_create(&c, NULL, consumer, (void*)rq);


	pthread_join(p, NULL);
	pthread_join(c, NULL);

	delete rq; //��������ʱҪɾ��
	return 0;
}



