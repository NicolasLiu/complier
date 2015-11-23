#include "global.h"
int errorNum;//错误数量

int error(int code)
{
	cout << "*******************************" << endl;
	cout << "第" << sLine << "行 error " << code << endl;
	cout << "*******************************" << endl;
	errorNum++;
	exit(code);
	return code;
}