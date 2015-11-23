#include "global.h"
void program()
{
	addSymTableLevel();
	childprogram();
	if (!(symbol.type == _fullpoint))
		error(22);//缺少.
	return;
}
void childprogram()
{
	getSym();
	if (symbol.type == _const)//const
	{
		constblock();
		getSym();
	}
	if (symbol.type == _var)//var
	{
		varblock();
	}
	while (1) {
		while (symbol.type == _procedure)//procedure
		{
			procedureblock();//过程说明部分
		}
		if (symbol.type == _function)//function
		{
			functionblock();//函数说明部分
		}
		else
		{
			break;
		}
	}
	if (symbol.type == _begin)
	{
		compound();//复合语句
	}
	else
	{
		error(23);//缺少begin
	}
}