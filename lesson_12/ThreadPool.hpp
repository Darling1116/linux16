#pragma once

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;

class Task{
	public:
		int base;
	public:
		Task(){}

		Task(int _b):base(_b){}

		void Run(){
			cout << "Thread id [" << pthread_self() << "]" << "task run ... done: base #" << base << "  pow is #" << pow(base, 2) << endl;
		}
		//pthread_self():��ȡ�߳��Լ���tid

		~Task(){}
};


class ThreadPool{
	private:
		queue<Task*> q;
		int max_cap;
		bool quit;  //�߳��Ƿ��˳���Ĭ�ϲ��˳�
		pthread_mutex_t lock;
		pthread_cond_t cond;
		
	private:
		void LockQueue(){
			pthread_mutex_lock(&lock);
		}

		void UnlockQueue(){
			pthread_mutex_unlock(&lock);
		}

		void ThreadWakeUp(){
			pthread_cond_signal(&cond);
		}

		void ThreadsWakeUp(){
			pthread_cond_broadcast(&cond);  
		}

		void ThreadWait(){
			pthread_cond_wait(&cond, &lock);
		}

		bool IsEmpty(){
			if(q.size()==0){
				return true;
			}
			else{
				return false;
			}
		}


	public:
		ThreadPool(int _cap):max_cap(_cap), quit(false){
		}
		

		void ThreadPoolInit(){
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&cond, nullptr);
			pthread_t tid;
			for(int i=0; i<max_cap; i++){
				pthread_create(&tid, nullptr, Routine, this);
			}
		}

		static void *Routine(void *arg){
			ThreadPool *tp = (ThreadPool*) arg;
			while(!tp->quit){
				tp->LockQueue();
				while(!tp->quit && tp->IsEmpty()){
					tp->ThreadWait();
				}
				Task t;
				if(!tp->quit && !tp->IsEmpty()){
					tp->Get(t);
				}
				tp->UnlockQueue();
				t.Run();
			}
		}

		void ThreadQuit(){  //�˳�֮��Ҫȥ֪ͨ��һ���������ߴ�������
			if(!IsEmpty()){  //����̳߳ز�Ϊ�գ��޷��˳�
				std::cout << "task queue is not empty..." << endl;
				return;
			}
			quit = true;
			ThreadsWakeUp();  //�̳߳�Ϊ�գ��߳��˳�ʱ֪ͨ�����ߴ�������
		}


		void Put(Task &in){  //���������
			LockQueue();
			q.push(&in);
			UnlockQueue();
			ThreadWakeUp();  //֪ͨ����������
			//ThreadsWakeUp();
		}

		void Get(Task &out){  //���������
			//LockQueue();
			Task *t = q.front();
			q.pop();
			out = *t;
			//UnlockQueue();
		}

		~ThreadPool(){
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&cond);
		}
};

