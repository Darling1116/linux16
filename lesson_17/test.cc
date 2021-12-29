#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//stringstream可以使int类型转为string类型
int main(){
	int a = 123;
	stringstream ss;
	ss << a;
	string str;
	ss >> str;
	cout << str << endl;

	return 0;
}