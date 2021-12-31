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
		//pthread_self():获取线程自己的tid

		~Task(){}
};



class ThreadPool{
	private:
		queue<Task*> q;
		int max_cap;
		pthread_mutex_t lock;
		pthread_cond_t cond; //针对消费者

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
			pthread_cond_signal(&cond); //一次只唤醒一个线程:按顺序唤醒
			//pthread_cond_broadcast(&cond); //采用广播信号的方式，随机唤起
		}

		void ThreadsWakeUp(){
			pthread_cond_broadcast(&cond);  //一次唤醒一批数据
		}

	

	public:

		ThreadPool(int cap):max_cap(cap){  //构造函数
		}

		void Put(Task &in){ //往任务队列里面放任务---server
			LockQueue();
			q.push(&in);
			UnlockQueue();
			ThreadWakeUp(); //任务放好之后，通知消费者处理任务
		}


		void Get(Task &out){ //从任务队列里面拿任务然后进行处理---Thread Pool
			//LockQueue();
			//out = q.front();
			//q.pop();
			Task *t = q.front();
			q.pop();
			out = *t;  //注意这里要用解引用！！！
			//UnlockQueue(); 
			//在Routine逻辑里面，处理任务过程中，已经进行了加锁保护的操作，所以这里不需要加锁操作了
		}

		static void *Routine(void *arg){ //内部成员函数，会默认增加this指针作为参数
		//但被static修饰的成员函数无法访问其他成员
		//所以在pthread_create中，把this指针作为第四个参数传进来
			ThreadPool *this_tp = (ThreadPool*) arg;

			while(true){
				this_tp->LockQueue();
				//访问临界资源：从任务队列拿任务---执行任务
				while(this_tp->IsEmpty()){
					this_tp->ThreadWait();
				}
				Task t;
				this_tp->Get(t);
				this_tp->UnlockQueue();
				t.Run();
			}
		}


		void ThreadPoolInit(){
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&cond, nullptr);

			//初始时，服务器端已经创建好了一堆线程
			pthread_t t;
			for(int i=0; i<max_cap; i++){
				pthread_create(&t, nullptr, Routine, this); ////注意！！！
			}
		}


		~ThreadPool(){
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&cond);
		}
};



//#endif
