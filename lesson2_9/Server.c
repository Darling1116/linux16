#include "common.h"

//ʵ�ֹ����ڴ���ԵĴ���
//key��֤ͨ�ŵ�����ʹ�õ���ͬһ�鹲���ڴ�

int main(){
key_t key = ftok(PATHNAME, PROJ_ID);
printf("key: %p\n", key);

int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL |0666);  //���������ڴ�
if(shmid < 0){
	printf("shmget error...\n");
	exit(1);
}
printf("shmid: %d\n", shmid);
//sleep(10);

char* str = (char*)shmat(shmid, NULL, 0);   //���������ڴ�
//sleep(5);

while(1){
	sleep(1);
	printf("%s\n", str);
}



shmdt(str);  //ȡ������

shmctl(shmid, IPC_RMID, NULL);  //ɾ�������ڴ�
return 0;
}