#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <math.h>

using namespace std;

class Task{
	public:
		int sock;
		std::map<std::string, std::string> dict;  
		//��̬��Ա����Ҫ�������ʼ��
	public:
		Task(){}

		Task(int _sock):sock(_sock){
			dict.insert(std::make_pair<std::string, std::string>("apple", "123456"));
		}

		void Run(){  //��һ���̴߳���һ������
			cout << "get a new link..." << pthread_self() << endl;
			char msg[1024];
			ssize_t s = recv(sock, msg, sizeof(msg)-1 ,0);
			if(s > 0){
				msg[s] = 0;
				std::cout << "client# "  << msg << std::endl;
				//std::string echo = msg;
				//echo += " [server echo!]";
				//send(sock, echo.c_str(), echo.size(), 0); 
				//---�̳߳��������ʵ�ַ��빦��---
				std::string key = msg;
				send(sock, dict[msg].c_str(), dict[key].size(), 0);
			}
			else if(s == 0){
				std::cout << "client quit..." << std::endl;
			}
			else{
				std::cout << "recv msg erroe...\n" << std::endl;
			}
		}

		~Task(){
			std::cout << "server close sock: " << sock << std::endl;
			close(sock);
		}
};



class ThreadPool{
	private:
		queue<Task*> q;
		int max_cap;
		pthread_mutex_t lock;
		pthread_cond_t cond; //���������
		bool quit;
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
		}

		void ThreadsWakeUp(){
			pthread_cond_broadcast(&cond);  //һ�λ���һ������
		}


	public:
		ThreadPool(int cap):max_cap(cap), quit(false){  //���캯��
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


		static void *Routine(void *arg){ //�ڲ���Ա��������Ĭ������thisָ����Ϊ����
		//����static���εĳ�Ա�����޷�����������Ա
		//������pthread_create�У���thisָ����Ϊ���ĸ�����������
			ThreadPool *this_tp = (ThreadPool*) arg;

			while(!this_tp->quit){
				this_tp->LockQueue();
				while(!this_tp->quit && this_tp->IsEmpty()){
					this_tp->ThreadWait();
				}
				Task *t;
				if(!this_tp->quit){
					this_tp->Get(&t);
				}
				this_tp->UnlockQueue();
				t->Run();
				delete t;  //����ִ����͸ɵ���
			}
		}


		void Put(Task &in){ //������������������---server
			LockQueue();
			q.push(&in);
			UnlockQueue();
			ThreadWakeUp(); //����ź�֮��֪ͨ�����ߴ�������
		}


		void Get(Task **out){ //�������������������Ȼ����д���---Thread Pool
			Task *t = q.front();
			q.pop();
			*out = t;  //ע������Ҫ�ý����ã�����
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

		~ThreadPool(){
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&cond);
		}
};


#endif
