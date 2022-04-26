#include "common.h"

int main(){
key_t key = ftok(PATHNAME, PROJ_ID);
//printf("key: %p\n", key);

int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL |0666);  //���������ڴ�
if(shmid < 0){
	printf("shmget erroe...\n");
	exit(1);
}
printf("shmid: %d\n", shmid);

shmctl(shmid, IPC_RMID, NULL);  //ɾ�������ڴ�
return 0;
}