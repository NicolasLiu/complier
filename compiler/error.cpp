#include "global.h"

int error(int code)
{
	cout << "*******************************" << endl;
	cout << "µÚ" << sLine << "ÐÐ error " << code << endl;
	cout << "*******************************" << endl;
	exit(code);
	return code;
}