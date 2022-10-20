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
	//����˽�г�Ա
	private:
		std::queue<int> q;
		int cap;
		pthread_mutex_t lock;
		pthread_cond_t p_cond;
		pthread_cond_t c_cond;

	//���幫�з���
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
		BlockQueue(int m_cap):cap(m_cap){ //��ʼ��
			pthread_mutex_init(&lock, nullptr);
			pthread_cond_init(&p_cond, nullptr);
			pthread_cond_init(&c_cond, nullptr);
	    }


		void Put(int &in){	//������
			LockQueue();
	
			if(IsFull()){ //����Ϊ�������������������ѣ������ߵȴ�����������������
				WakeUpConsumer();
				std::cout << "queue is full, notify consumer, productor stop." << std::endl;
				ProductorWait();
			}
			//����
			q.push(in);

			UnlockQueue();

		}


		void Get(int &out){  //������
			LockQueue();
			
			//if(IsEmpty()){  //����Ϊ�գ����������������������ߵȴ�����������������
			if(q.size() <= cap){
				WakeUpProductor();
				std::cout << "queue is empty, notify productor, consumer stop" << std::endl;
				ConsumerWait();
			}
			//����
			out = q.front();
			q.pop();

			UnlockQueue();

		}


		~BlockQueue(){ //����
			pthread_mutex_unlock(&lock);
			pthread_cond_destroy(&p_cond);
			pthread_cond_destroy(&c_cond);
		}

};





#endif
