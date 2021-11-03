#include "BlockQueue.hpp"


	void *productor(void *arg){
		BlockQueue *qp = (BlockQueue*)arg;
		//生产者放数据
		//srand((unsigned long)time(NULL));
		while(true){
			int data = rand()%10+1;
			qp->Put(data);
			std::cout << "productor data done:..." << data << std::endl;
		}
	}

	
	void *consumer(void *arg){
		BlockQueue *qc = (BlockQueue*)arg;
		//消费者取数据
		int data;
		while(true){
            sleep(1);
			qc->Get(data);
			std::cout << "consumer data done:..." << data << std::endl;
		}
	}


int main(){

	//int cap = 5;
	BlockQueue *q = new BlockQueue(5);

	//创建分别代表生产者和消费者的线程
	pthread_t ptid;
	pthread_t ctid;
	pthread_create(&ptid, nullptr, productor, (void*)q); //新线程的逻辑参数为阻塞队列
	pthread_create(&ctid, nullptr, consumer, (void*)q);

	pthread_join(ptid, nullptr);
	pthread_join(ctid, nullptr);
	
	delete q;
	return 0;
}
