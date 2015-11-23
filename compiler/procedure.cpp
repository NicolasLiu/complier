#include "global.h"
void procedureblock()
{
	while (1)
	{
		procedurehead();
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
		symTableItem sym = { procedurename,{ procedurename,0,_procedure,0,0,0,0,0 } };
		insertSymTable(sym);
		addSymTableLevel();
		gen_icode(q_procedure, {}, {}, { 0,0,procedurename });
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
				error(12);//缺少)
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
			updateSymTable(sym);
			return;
		}
		else
		{
			error(11);//缺少;
		}
	}
	else
	{
		error(24);//非法的过程标识符
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
			else
			{
				error(26);//非法的参数类型
			}
		}
		else
		{
			error(25);//非法的参数
		}
	}

	error(25);//非法的参数

}