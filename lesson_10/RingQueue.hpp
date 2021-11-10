#ifndef __RING_QUEUE_H__
#define __RING_QUEUE_H__

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Task
{
	public:
		int m;
		int n;

	public:
		Task(){
		}

		Task(int _m, int _n):m(_m), n(_n){
		}

		int Run(){
			int sum = m*n;
			return sum;
		}

		~Task(){
		}
};


class RingQueue
{
	private:
		//vector<int> v;
		vector<Task> v;
		int cap;
		sem_t sem_blank;
		sem_t sem_data;
		int p_index;
		int c_index;


	private:
		void P(sem_t &s){
			sem_wait(&s);
		}

		void V(sem_t &s){
			sem_post(&s);
		}



	public:
		RingQueue(int _cap):cap(_cap),v(_cap){
			sem_init(&sem_blank,0,_cap); //注意sem信号中init函数的参数形式
			sem_init(&sem_data,0,0);
			p_index = 0;
			c_index = 0;
		}
		

		//void Put(int &in){
		void Put(Task &t_in){
			P(sem_blank);
			v[p_index] = t_in;
			p_index++;
			p_index %= cap;
			V(sem_data);
		}


		//void Get(int &out){
		void Get(Task &t_out){
			P(sem_data);
			t_out = v[c_index];
			c_index++;
			c_index %= cap;
			V(sem_blank);
		}


		~RingQueue(){
			//注意：这里一定要销毁数据，再销毁格子
			sem_destroy(&sem_data);
			sem_destroy(&sem_blank);

			p_index = 0;
			c_index = 0;
		}

};


#endif