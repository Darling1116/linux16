#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;

class Task{
	public:
		//int base;
		int sock;
	public:
		Task(){}

		Task(int _sock):sock(_sock){}

		void Run(){
			//cout << "Thread id [" << pthread_self() << "]" << "task run ... done: base #" << base << "  pow is #" << pow(base, 2) << endl;
			cout << "Task is running..." << endl;
			char msg[1024];
			ssize_t s = recv(sock, msg, sizeof(msg)-1 ,0);
			if(s > 0){
				msg[s] = 0;
				std::cout << "client# "  << msg << std::endl;
				std::string echo = msg;
				echo += " [server echo!]";
				send(sock, echo.c_str(), echo.size(), 0); 
			}
			else if(s == 0){
				std::cout << "client quit...\n" << std::endl;
			}
			else{
				std::cout << "recv msg erroe...\n" << std::endl;
			}
		}

		~Task(){
			std::cout << "server close sock...\n" << std::endl;
			close(sock);
		}
};



class ThreadPool{
	private:
		queue<Task*> q;
		int max_cap;
		pthread_mutex_t lock;
		pthread_cond_t cond; //���������
	private:
		void LockQueue(){
			pthread_mutex_lock(&lock);
		}

		void UnlockQueue(){
			pthread_mutex_unlock(&lock);
		}

		bool IsEmpty(){
			if(q.size()==0)
				return true;
			else
				return false;
		}

		void ThreadWait(){
			pthread_cond_wait(&cond, &lock);
		}

		void ThreadWakeUp(){
			pthread_cond_signal(&cond); //һ��ֻ����һ���߳�
			//pthread_cond_broadcast(&cond); //���ù㲥�źŵķ�ʽ���������
		}

		void ThreadsWakeUp(){
			pthread_cond_broadcast(&cond);  //һ�λ���һ������
		}


	public:
		ThreadPool(int cap):max_cap(cap){  //���캯��
		}

		void Put(Task &in){ //������������������---server
			LockQueue();
			q.push(&in);
			UnlockQueue();
			ThreadWakeUp(); //����ź�֮��֪ͨ�����ߴ�������
		}


		void Get(Task **out){ //�������������������Ȼ����д���---Thread Pool
			//LockQueue();
			//out = q.front();
			//q.pop();
			Task *t = q.front();
			q.pop();
			*out = t;  //ע������Ҫ�ý����ã�����
			//UnlockQueue(); 
			//��Routine�߼����棬������������У��Ѿ������˼��������Ĳ������������ﲻ��Ҫ����������
		}


		void ThreadQuit(){ //�˳�֮��Ҫȥ֪ͨ��һ���������ߴ�������
			/*
			if(!IsEmpty()){
				cout << "task queue is not empty" << endl;  //�������Ϊ�ǿգ���Ҫ������������
				return;
			}
			quit = true;
			ThreadsWakeUp(); //�������Ϊ�գ�֪ͨ�����ߴ�������
			*/
		}

		static void *Routine(void *arg){ //�ڲ���Ա��������Ĭ������thisָ����Ϊ����
		//����static���εĳ�Ա�����޷�����������Ա
		//������pthread_create�У���thisָ����Ϊ���ĸ�����������
			ThreadPool *this_tp = (ThreadPool*) arg;

			while(true){
				this_tp->LockQueue();
				//�����ٽ���Դ�����������������---ִ������
				while(this_tp->IsEmpty()){
					this_tp->ThreadWait();
				}
				Task *t;
				this_tp->Get(&t);
				this_tp->UnlockQueue();
				t->Run();
				delete t;
			}
		}

		void ThreadPoolInit(){
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&cond, nullptr);
			//��ʼʱ�����������Ѿ���������һ���߳�
			pthread_t t;
			for(int i=0; i<max_cap; i++){
				pthread_create(&t, nullptr, Routine, this); ////ע�⣡����
			}
		}


		~ThreadPool(){
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&cond);
		}
};


#endif
