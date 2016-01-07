#include "global.h"
void procedureblock()
{
	while (1)
	{
		procedurehead();
		if (errorMark)
		{
			errorMark = 0;
			getSym();
		}
		childprogram();
		if (symbol.type == _semicolon)
		{
			popSymTableLevel();
			getSym();
			if (symbol.type != _procedure)
			{
				break;
			}
		}
		else
		{
			error(11);//缺少;
			return;
		}
	}
	
}
void procedurehead()
{
	char procedurename[100];
	int params[MaxParams][2];
	char paramName[MaxParams][100];
	int num = 0;
	getSym();
	if (symbol.type == _identifier)
	{
		symItem symitem = findSymTableLocal(symbol.identifier);
		if (!symitem.name.empty())
		{
			error(42);//重定义的标识符
		}
		strcpy(procedurename, symbol.identifier);
		symTableItem sym = { procedurename,{ procedurename,0,_procedure,0,0,0,0,0 } };
		insertSymTable(sym);
		addSymTableLevel();
		gen_icode(q_procedure, {}, {}, { _procedure,0,0,procedurename });
		getSym();
		if (symbol.type == _lparenthese)
		{
			
			do 
			{
				formalparam(params, paramName, &num);
				if (errorMark)
				{
					return;
				}
				getSym();
			} while (symbol.type == _semicolon);
			if (symbol.type == _rparenthese)
			{
				getSym();
			}
			else
			{
				error(12);//缺少)
				return;
			}
			
		}

		if (symbol.type == _semicolon)
		{
			symItem s = { procedurename,0,_procedure,0,0,0,0,num };
			for (int i = 0; i < num; i++)
			{
				s.params[i][0] = params[i][0];
				s.params[i][1] = params[i][1];
			}
			symTableItem sym = { procedurename, s};
			updateSymTable(sym);
			for (int i = 0; i < num; i++)
			{
				gen_icode(q_alloc, {}, {}, { s.params[i][1],0,s.params[i][0],paramName[i] });
			}
			return;
		}
		else
		{
			error(11);//缺少;
			return;
		}
	}
	else
	{
		error(24);//非法的过程标识符
		return;
	}
}
void formalparam(int params[][2], char paramName[][100], int *num)
{
	int isVar = 0, n = 0;
	getSym();
	if (symbol.type == _var)
	{
		isVar = 1;
		getSym();
	}
	while (symbol.type == _identifier)
	{
		symItem sym = findSymTableLocal(symbol.identifier);
		if (!sym.name.empty())
		{
			error(42);//重定义的标识符
		}
		params[*num][0] = isVar;
		strcpy(paramName[*num], symbol.identifier);
		n++; (*num)++;
		getSym();
		if (symbol.type == _comma)
		{
			getSym();
		}
		else if (symbol.type == _colon)
		{
			getSym();
			if (symbol.type == _integer || symbol.type == _char)
			{
				for (int i = 0; i < n; i++)
				{						
					symTableItem sym = { paramName[*num - i - 1],{ paramName[*num - i - 1],_var,symbol.type,0,0,0,isVar } };
					insertSymTable(sym);
					params[*num - i - 1][1] = symbol.type;
				}
				return;
			}
			else
			{
				error(26);//非法的参数类型
				return;
			}
		}
		else
		{
			error(25);//非法的参数
			return;
		}
	}

	error(25);//非法的参数
	return;
}