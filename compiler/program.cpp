#include "global.h"
void program()
{
	addSymTableLevel();
	childprogram();
	if (!(symbol.type == _fullpoint))
		error(22);//ȱ��.
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
	if (symbol.type == _begin)
	{
		compound();//�������
	}
	else
	{
		error(23);//ȱ��begin
	}
}