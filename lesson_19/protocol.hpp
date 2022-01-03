#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <iostream>

//定义双方用于通信的协议

typedef struct request{
	int x;
	int y;
	char op; //操作符：+ - * / %
}request_t;


typedef struct response{
	int code;  //正常计算时，code默认为0
	int result;
}response_t;


#endif