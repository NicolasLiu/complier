#include "global.h"
int errorNum;//��������

int error(int code)
{
	cout << "*******************************" << endl;
	cout << "��" << sLine << "�� error " << code << endl;
	cout << "*******************************" << endl;
	errorNum++;
	exit(code);
	return code;
}