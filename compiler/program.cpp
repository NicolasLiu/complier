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
			procedureblock();//����˵������
		}
		if (symbol.type == _function)//function
		{
			functionblock();//����˵������
		}
		else
		{
			break;
		}
	}
	compound();//�������
	
}