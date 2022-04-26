#include "common.h"

int main(){
key_t key = ftok(PATHNAME, PROJ_ID);
printf("key: %p\n", key);

int shmid = shmget(key, SIZE, 0);  //���������ڴ�
if(shmid < 0){
	printf("shmget erroe...\n");
	exit(1);
}
printf("shmid: %d\n", shmid);

char* str = (char*)shmat(shmid, NULL, 0);   //���������ڴ�

char c = 'a';
for(; c<='z'; c++){
	str[c-'a'] = c;  //ÿ��5���ӣ��������ڴ���д��һ���ַ�
	sleep(5);
}


shmdt(str);  //ȡ������

shmctl(shmid, IPC_RMID, NULL);  //ɾ�������ڴ�
return 0;
}