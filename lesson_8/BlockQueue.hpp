#ifndef __BLOCK_QUEUE_H__
#define __BLOCK_QUEUE_H__

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <queue>
#include <pthread.h>

using namespace std;

//增加任务类：让线程处理任务
class Task
{
	public:
		int x;
		int y;
	
	public:
		Task(){
		}

		Task(int x1, int y1):x(x1), y(y1){
		}

		int Run(){
			int sum = x+y;
			return sum;
		}

		~Task(){
		}
};


class BlockQueue
{
	private:
		//queue<int> q;
		queue<Task>q;
		int cap;
		pthread_mutex_t lock;
		pthread_cond_t p_cond;
		pthread_cond_t c_cond;

	
	public:
		void QueueLock(){
			pthread_mutex_lock(&lock);
		}

		void QueueUnlock(){
			pthread_mutex_unlock(&lock);
		}


		bool IsFull(){
			if(q.size()>=cap){
				return true;
			}
			return false;
		
		}


		void ProductorWait(){
			cout << "productor wait..." << endl;
			pthread_cond_wait(&p_cond, &lock);
			
		}


		void ConsumerSignal(){
			cout << "consumer signal..." << endl;
			pthread_cond_signal(&c_cond);
			
		}

		bool IsEmpty(){
			if(q.size()==0){
				return true;
			}
			return false;
		}

		void ProductorSignal(){
			cout << "productor signal..." << endl;
			pthread_cond_signal(&p_cond);
			
		}

		void ConsumerWait(){
			cout << "consumer wait..." << endl;
			pthread_cond_wait(&c_cond, &lock);
			
		}
	

	public:
		BlockQueue(int m_cap):cap(m_cap){
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&p_cond, nullptr);
			pthread_cond_init(&c_cond, nullptr);
		}


		//void Put(int &in){
		void Put(Task &t){
			QueueLock();
			if(IsFull()){  //一定要先唤醒，再等待
				ConsumerSignal();
				ProductorWait();
			}
			//q.push(in);
			q.push(t);
			QueueUnlock();
		}


		//void Get(int &out){  
		void Get(Task &t){  //Get时，参数一定要取地址
			QueueLock();
			if(IsEmpty()){
				ProductorSignal();
				ConsumerWait();
			}
			//out = q.front();
			t = q.front();
			q.pop();
			QueueUnlock();

		}

		~BlockQueue(){
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&p_cond);
			pthread_cond_destroy(&c_cond);
		}

		
};

#endif