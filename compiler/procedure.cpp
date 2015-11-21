#include "global.h"
void procedureblock()
{
	addSymTableLevel();
	while (1)
	{
		procedurehead();
		childprogram();
		getSym();
		if (symbol.type == _comma)
		{
			getSym();
			if (symbol.type != _procedure)
			{
				break;
			}
		}
		else
		{
			error(1);//应以分号结束
		}
	}
	
}
void procedurehead()
{
	char procedurename[100];
	char params[10][100];
	int num = 0;
	getSym();
	if (symbol.type == _identifier)
	{
		strcpy(procedurename, symbol.identifier);
		getSym();
		if (symbol.type == _lparenthese)
		{
			
			do 
			{
				formalparam(params, &num);
				getSym();
			} while (symbol.type == _semicolon);
			if (symbol.type == _rparenthese)
			{
				getSym();
			}
			else
			{
				error(1);//过程说明部分缺少右括号
			}
			
		}

		if (symbol.type == _semicolon)
		{
			symItem s = { procedurename,0,_procedure,0,0,0,0,num };
			for (int i = 0; i < num; i++)
			{
				strcpy(s.params[i], params[i]);
			}
			symTableItem sym = { procedurename, s};
			insertSymTable(sym);
			return;
		}
		else
		{
			error(1);//应以分号结束
		}
	}
	else
	{
		error(1);//过程标识符错误
	}
}
void formalparam(char params[][100], int *num)
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
		strcpy(params[*num], symbol.identifier);
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
					symTableItem sym = { params[*num - i - 1],{ params[*num - i - 1],_var,symbol.type,0,0,0,isVar } };
					insertSymTable(sym);
				}
				return;
			}
		}
		else
		{
			error(1);//过程或函数参数错误
		}
	}

	error(1);//过程或函数参数错误

}