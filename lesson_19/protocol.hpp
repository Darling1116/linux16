#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <iostream>

//����˫������ͨ�ŵ�Э��

typedef struct request{
	int x;
	int y;
	char op; //��������+ - * / %
}request_t;


typedef struct response{
	int code;  //��������ʱ��codeĬ��Ϊ0
	int result;
}response_t;


#endif