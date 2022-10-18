#include "common.h"

//实现共享内存测试的代码
//key保证通信的两端使用的是同一块共享内存

int main(){
key_t key = ftok(PATHNAME, PROJ_ID);
printf("key: %p\n", key);

int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL |0666);  //创建共享内存
if(shmid < 0){
	printf("shmget error...\n");
	exit(1);
}
printf("shmid: %d\n", shmid);
//sleep(10);

char* str = (char*)shmat(shmid, NULL, 0);   //关联共享内存
//sleep(5);

while(1){
	sleep(1);
	printf("%s\n", str);
}



shmdt(str);  //取消关联

shmctl(shmid, IPC_RMID, NULL);  //删除共享内存
return 0;
}