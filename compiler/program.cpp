#include "global.h"
void program()
{
	childprogram();
	getSym();
	if (!(symbol.type == _comma))
		error(3);
	return;
}
void childprogram()
{
	addSymTableLevel();
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
	compound();//复合语句
	
}