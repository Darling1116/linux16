#include "common.h"

int main(){
key_t key = ftok(PATHNAME, PROJ_ID);
printf("key: %p\n", key);

int shmid = shmget(key, SIZE, 0);  //创建共享内存
if(shmid < 0){
	printf("shmget erroe...\n");
	exit(1);
}
printf("shmid: %d\n", shmid);

char* str = (char*)shmat(shmid, NULL, 0);   //关联共享内存

char c = 'a';
for(; c<='z'; c++){
	str[c-'a'] = c;  //每隔5秒钟，往共享内存里写入一个字符
	sleep(5);
}


shmdt(str);  //取消关联

shmctl(shmid, IPC_RMID, NULL);  //删除共享内存
return 0;
}