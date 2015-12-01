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
		for (unordered_map<string, symItem>::iterator iter = symbolTable.back().begin(); iter != symbolTable.back().end(); ++iter)
		{
			if (iter->second.constvar == _var && (iter->second.type == _integer || iter->second.type == _char))
			{
				gen_icode(q_local, {}, {}, { iter->second.type,0,0,iter->second.name, iter->second.dimension, iter->second.upperbound });
			}
		}
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
		gen_icode(q_begin, {}, {}, {});
		compound();//复合语句
		gen_icode(q_end, {}, {}, {});
	}
	else
	{
		error(23);//缺少begin
	}
}