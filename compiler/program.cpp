#include "global.h"
void program()
{
	childprogram();
	getSym();
	if (!symbol.type == _comma)
		error(3);
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
		getSym();
	}
	while (1) {
		while (symbol.type == _procedure)//procedure
		{
			procedureblock();//����˵������
			getSym();
		}
		if (symbol.type == _function)//function
		{
			functionblock();//����˵������
			getSym();
		}
		else
		{
			break;
		}
	}
	compound();//�������
	
}