#include "global.h"
void constblock()
{
	do 
	{
		constdefine();
		getSym();
	} while (symbol.type == _comma);
	if (symbol.type == _semicolon)
	{
		return;
	}
	else
	{
		error(11);//缺少;
	}
	
}
void constdefine()
{
	char name[100];
	int value;
	getSym();
	if (symbol.type == _identifier)//标识符
	{
		symItem sym = findSymTableLocal(symbol.identifier);
		if (!sym.name.empty())
		{
			error(42);//重定义的标识符
		}
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
				symTableItem sym = { name,{name,_const,_integer,value} };
				insertSymTable(sym);
			}
			else if (symbol.type == _character)//字符
			{
				value = k*symbol.value;
				symTableItem sym = { name,{ name,_const,_char,value } };
				insertSymTable(sym);
			}
			else
			{
				error(30);//非法的无符号整数
			}
		}
		else
		{
			error(14);//缺少=
		}
	}
	else
	{
		error(29);//非法的标识符
	}

}