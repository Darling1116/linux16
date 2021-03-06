#include "str.h"

void str_trim_crlf(char* str){
	assert(str != NULL);
	char *p = str + (strlen(str)-1);
	//先让p指向最后一个位置
	while(*p=='\n' || *p=='\r')
		*p-- = '\0';
}

void str_split(const char *str, char *left, char *right, char c){
	assert(str != NULL);
	char *pos = strchr(str, c);
	if(pos == NULL)
		strcpy(left, str);
	else{
		strncpy(left, str, pos-str);
		strcpy(right, pos+1);
	}
}