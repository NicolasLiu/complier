#include "global.h"

int error(int code)
{
	cout << "*******************************" << endl;
	cout << "��" << sLine << "�� error " << code << endl;
	cout << "*******************************" << endl;
	exit(code);
	return code;
}