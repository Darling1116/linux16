#include "BlockQueue.hpp"


	void *productor(void *arg){
		BlockQueue *qp = (BlockQueue*)arg;
		//�����߷�����
		//srand((unsigned long)time(NULL));
		while(true){
			int data = rand()%10+1;
			qp->Put(data);
			std::cout << "productor data done:..." << data << std::endl;
		}
	}

	
	void *consumer(void *arg){
		BlockQueue *qc = (BlockQueue*)arg;
		//������ȡ����
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

	//�����ֱ���������ߺ������ߵ��߳�
	pthread_t ptid;
	pthread_t ctid;
	pthread_create(&ptid, nullptr, productor, (void*)q); //���̵߳��߼�����Ϊ��������
	pthread_create(&ctid, nullptr, consumer, (void*)q);

	pthread_join(ptid, nullptr);
	pthread_join(ctid, nullptr);
	
	delete q;
	return 0;
}
