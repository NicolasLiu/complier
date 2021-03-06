#include "global.h"
void functionblock()
{
	while (1)
	{
		functionhead();
		if (errorMark)
		{
			errorMark = 0;
		}
		childprogram();
		if (symbol.type == _semicolon)
		{
			popSymTableLevel();
			getSym();
			if (symbol.type != _function)
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
void functionhead()
{
	char functionname[100];
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
		strcpy(functionname, symbol.identifier);
		symTableItem sym = { functionname,{ functionname,0,_function,0,0,0,0,0 } };
		insertSymTable(sym);
		addSymTableLevel();
		gen_icode(q_function, {}, {}, { _function,0,0,functionname });
		getSym();
		if (symbol.type == _lparenthese)
		{

			do
			{
				formalparam(params, paramName, &num);
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
		if (symbol.type == _colon)
		{
			getSym();
			if (symbol.type == _integer || symbol.type == _char)
			{
				int rtnType = symbol.type;
				getSym();
				if (symbol.type == _semicolon)
				{
					symItem s = { functionname,0,_function,rtnType,0,0,0,num };
					for (int i = 0; i < num; i++)
					{
						s.params[i][0] = params[i][0];
						s.params[i][1] = params[i][1];
					}
					symTableItem sym = { functionname, s };
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
				error(28);//非法的返回值类型
				return;
			}
			
		}
		else
		{
			error(13);//缺少:
			return;
		}
		
	}
	else
	{
		error(27);//非法的函数标识符
		return;
	}
}