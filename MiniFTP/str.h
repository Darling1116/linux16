#ifndef _STR_H_
#define _STR_H_

#include "common.h"

void str_trim_crlf(char* str);
//×Ö·û´®²Ã¼ô

void str_split(const char *str, char *left, char *right, char c);
//×Ö·û´®·Ö¸î

void str_upper(char *str);

#endif /* _STR_H_ */