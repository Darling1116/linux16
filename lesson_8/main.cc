#include "BlockQueue.hpp"

//1. ������������
//2. �����Ͷ���
void *p_task(void *arg){
	BlockQueue *qp = (BlockQueue*)arg;
	/*
	int in;
	while(true){
		//sleep(1);
		in = rand()%10+1;
		qp->Put(in);
		cout << "id: " << pthread_self() << " p_task done...: " << in << endl;
	}
	*/
	while(true){
		int x = rand()%10+1;
		int y = rand()%100+1;
		Task t(x, y);  //����һ������
		qp->Put(t);  //���������������������������
		cout << "p_task Task is: " << x << "+" << y << "= " << "?" << endl;
	}


}


void *c_task(void *arg){
	BlockQueue *qc = (BlockQueue*)arg;
	/*
	int out;
	while(true){
		sleep(2);
		qc->Get(out);
		cout << "id: " << pthread_self() << " c_task done...:" << out << endl;
	}
	*/

	while(true){
		//sleep(1);
		Task t;
		qc->Get(t);
		//t.Run();
		cout << "c_task Task is:" << t.x << "+" << t.y << "=" << t.Run() << endl;
	}

}




int main(){
	BlockQueue *bq = new BlockQueue(5);

	pthread_t p_tid;
	pthread_t c_tid;

	pthread_create(&p_tid, nullptr, p_task, (void*)bq);
	//pthread_create(&p_tid, nullptr, p_task, (void*)bq);

	pthread_create(&c_tid, nullptr, c_task, (void*)bq);
	//pthread_create(&c_tid, nullptr, c_task, (void*)bq);


	pthread_join(p_tid, nullptr);
	pthread_join(c_tid, nullptr);

	delete bq; //���Ҫ�ͷŶ�����Դ
	return 0;
}
