#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <signal.h>

//��ͨ�źţ�1-31��

void show_pending(sigset_t *pending){
	int sig=1;
	for(; sig<=31; sig++){
		if(sigismember(pending, sig)){
			 printf("1");
		}
		else{
			printf("0");
		}
	}
	printf("\n");
}

void handler(int signo){
	printf("catch a signo: %d\n");
}


int main(){

//ctrl+C֮��2���źŲ���
//��ʱ����ʹ���źŲ�׽��������ȡ����pending���״̬��Ϣ
signal(2, handler);


//����2���źż�:
//һ�����յ�2���ź�(δ��״̬)����ӦPending���е����ݾͻ���0��Ϊ1
sigset_t block;
sigset_t oblock;
sigemptyset(&block);
sigemptyset(&oblock);
sigaddset(&block, 2);

//�������ź�������֮ǰ���Ȱ�ԭ�����ź������ֱ��ݵ�oblock��
sigprocmask(SIG_SETMASK, &block, &oblock);


//չʾpending��
sigset_t pending;
int count = 0;
while(1){
	sigemptyset(&pending);
	sigpending(&pending);  //��ȡ��ǰ���̵�δ��pending�źż�
	show_pending(&pending);
	sleep(1);
	count++;
	if(count == 10){
		printf("recover sig mask!\n");
		sigprocmask(SIG_SETMASK, &oblock, NULL);
	}
	
}

}