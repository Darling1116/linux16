#ifndef __RING_QUEUE_H__
#define __RING_QUEUE_H__

#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>  //POSIX�ź�����ͷ�ļ�


//#define NUM 10

using namespace std;

//������������

class RingQueue
{
	private:
		vector<int> vec;  //����������ʵ�ֻ��ζ��У�����ʹ��c++�е�vector������
		int max_cap;

		//�����ź������䱾�����һ��������
		sem_t sem_blank;  //���ӣ�������
		sem_t sem_data;  //���ݣ�������

		int p_index;  //���������λ�õ��±�
		int c_index;  //���������λ�õ��±�


	private:
		void P(sem_t &s){  //POSIX�ź����У�P������sem_wait()---count--;
			sem_wait(&s);

		}

		void V(sem_t &s){  //POSIX�ź����У�V������sem_post()---count++;
			sem_post(&s);
		
		}


	public:
		//ע�⣺���ﻹҪ��vector���и�ֵ��ʼ������
		RingQueue(int cap):max_cap(cap),vec(cap){  
			sem_init(&sem_blank,0,max_cap); //��ʼʱ�����ζ�����ȫ�ǿո�
			sem_init(&sem_data,0,0);  //��ʼʱ�����ζ�����û������
			p_index = 0;
			c_index = 0;
		}

		void Put(int &in){	//���ø���
			P(sem_blank); //����
			//����
			vec[p_index] = in;
			p_index++;
			p_index %= max_cap; //�˳�
			V(sem_data);

		}


		void Get(int &out){  //��ȡ����
			P(sem_data); //����
			//����
			out = vec[c_index];
			c_index++;
			c_index %= max_cap;
			V(sem_blank); //�˳�

		}

		~RingQueue(){
			sem_destroy(&sem_data);
			sem_destroy(&sem_blank);
			//p_index = 0;
			//c_index = 0;

		}

};



#endif