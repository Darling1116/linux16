#ifndef __QUEUE_BLOCK_H__
#define __QUEUE_BLOCK_H__

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <math.h>

#define NUM 5

class BlockQueue{
	//定义私有成员
	private:
		std::queue<int> q;
		int cap;
		pthread_mutex_t lock;
		pthread_cond_t p_cond;
		pthread_cond_t c_cond;

	//定义公有方法
	public:
		void LockQueue(){
            pthread_mutex_lock(&lock);
		}

		void UnlockQueue(){
			pthread_mutex_unlock(&lock);
		}

		void WakeUpConsumer(){
            std::cout << "wake up consumer..." << std::endl;
			pthread_cond_signal(&c_cond);

		}


		void ProductorWait(){
            std::cout << "productor wait..." << std::endl;
			pthread_cond_wait(&p_cond, &lock);

		}


		void WakeUpProductor(){
            std::cout << "wake up productor..." << std::endl;
			pthread_cond_signal(&p_cond);

		}



		void ConsumerWait(){
            std::cout << "consumer wait..." << std::endl;
			pthread_cond_wait(&c_cond, &lock);
		
		}

	
		bool IsFull(){
			if(q.size()>=cap){
				return true;
			}
			return false;
		}

		bool IsEmpty(){
			if(q.size()==0){
				return true;
			}
			return false;
		}


	public:
		BlockQueue(int m_cap):cap(m_cap){ //初始化
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&p_cond, nullptr);
			pthread_cond_init(&c_cond, nullptr);
	    }


		void Put(int &in){	//生产者
			LockQueue();
	
			if(IsFull()){ //队列为满，唤醒消费者来消费，生产者等待；否则，生产者生产
				WakeUpConsumer();
				std::cout << "queue is full, notify consumer, productor stop." << std::endl;
				ProductorWait();
			}
			//生产
			q.push(in);

			UnlockQueue();

		}


		void Get(int &out){  //消费者
			LockQueue();
			
			//if(IsEmpty()){  //队列为空，唤醒生产者生产，消费者等待；否则，消费者消费
			if(q.size() <= cap){
				WakeUpProductor();
				std::cout << "queue is empty, notify productor, consumer stop" << std::endl;
				ConsumerWait();
			}
			//消费
			out = q.front();
			q.pop();

			UnlockQueue();

		}


		~BlockQueue(){ //销毁
			pthread_mutex_unlock(&lock);
			pthread_cond_destroy(&p_cond);
			pthread_cond_destroy(&c_cond);
		}

};





#endif
