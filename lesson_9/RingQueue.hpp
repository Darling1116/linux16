#ifndef __RING_QUEUE_H__
#define __RING_QUEUE_H__

#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>  //POSIX信号量的头文件


//#define NUM 10

using namespace std;

//单生产单消费

class RingQueue
{
	private:
		vector<int> vec;  //利用数组来实现环形队列，这里使用c++中的vector迭代器
		int max_cap;

		//利用信号量，其本身就是一个计数器
		sem_t sem_blank;  //格子：生产者
		sem_t sem_data;  //数据：消费者

		int p_index;  //标记生产者位置的下标
		int c_index;  //标记消费者位置的下标


	private:
		void P(sem_t &s){  //POSIX信号量中，P操作用sem_wait()---count--;
			sem_wait(&s);

		}

		void V(sem_t &s){  //POSIX信号量中，V操作用sem_post()---count++;
			sem_post(&s);
		
		}


	public:
		//注意：这里还要对vector进行赋值初始化操作
		RingQueue(int cap):max_cap(cap),vec(cap){  
			sem_init(&sem_blank,0,max_cap); //初始时，环形队列里全是空格
			sem_init(&sem_data,0,0);  //初始时，环形队列里没有数据
			p_index = 0;
			c_index = 0;
		}

		void Put(int &in){	//放置格子
			P(sem_blank); //访问
			//生产
			vec[p_index] = in;
			p_index++;
			p_index %= max_cap; //退出
			V(sem_data);

		}


		void Get(int &out){  //获取数据
			P(sem_data); //访问
			//消费
			out = vec[c_index];
			c_index++;
			c_index %= max_cap;
			V(sem_blank); //退出

		}

		~RingQueue(){
			sem_destroy(&sem_data);
			sem_destroy(&sem_blank);
			//p_index = 0;
			//c_index = 0;

		}

};



#endif