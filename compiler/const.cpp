#include "global.h"
void constblock()
{

}
void constdefine()
{
	char name[100];
	int value;
	getSym();
	if (symbol.type == _identifier)//标识符
	{
		int k = 1;//符号默认为正
		strcpy(name, symbol.identifier);
		getSym();
		if (symbol.type == _equal)//=
		{
			getSym();
			if (symbol.type == _plus)//+
			{
				getSym();
				k = 1;
			}
			else if (symbol.type == _sub)//-
			{
				getSym();
				k = -1;
			}

			if (symbol.type == _constant)//无符号整数
			{
				value = k*symbol.value;
			}
			else
			{
				error(4);//不是无符号整数
			}
		}
		else
		{
			error(4);//不是等号
		}
	}
	else
	{
		error(4);//不是标识符
	}

}