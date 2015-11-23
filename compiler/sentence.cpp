#include "global.h"
void read()
{
	getSym();
	if (symbol.type == _lparenthese)
	{
		do 
		{
			getSym();
			if (symbol.type == _identifier)
			{
				getSym();
			}
			else
			{
				error(1);//非法的标识符
			}
		}
		while (symbol.type == _comma);
		if (symbol.type == _rparenthese)
		{
			getSym();
			return;
		}
		else
		{
			error(1);//应以分号结尾
		}
	}

}
void write()
{
	getSym();
	if (symbol.type == _lparenthese)
	{
		getSym();
		if (symbol.type == _string)
		{
			getSym();
			if (symbol.type == _comma)
			{
				getSym();
			}
			else
			{
				if (symbol.type == _rparenthese)
				{
					return;
				}
				else
				{
					error(1);//写语句缺少右括号
				}
			}
		}
		expression();
		if (symbol.type == _rparenthese)
		{
			getSym();
			return;
		}
		else
		{
			error(1);//写语句缺少右括号
		}
	}
}
void compound()
{
	do 
	{
		getSym();
		sentence();
	} while (symbol.type == _semicolon);
	if (symbol.type == _end)
	{
		getSym();
		return;
	} 
	else
	{
		error(1);//缺少end
	}
}
void forloop()
{
	getSym();
	if (symbol.type == _identifier)
	{
		getSym();
		if (symbol.type == _assign)
		{
			getSym();
			expression();
			if (symbol.type == _to || symbol.type == _downto)
			{
				getSym();
				expression();
				if (symbol.type == _do)
				{
					getSym();
					sentence();
					return;
				}
				else
				{
					error(1);//缺少do
				}
			}
			else
			{
				error(1);//缺少to或downto
			}
		}
		else
		{
			error(1);//缺少:=
		}
	}
	else
	{
		error(1);//非法的标识符
	}
}
void dowhile()
{
	sentence();
	getSym();
	if (symbol.type == _while)
	{
		getSym();
		expression();
		if (symbol.type == _equal || symbol.type == _less || symbol.type == _lessequal || symbol.type == _lessmore || symbol.type == _more || symbol.type == _moreequal)
		{
			getSym();
			expression();
			return;
		}
		else
		{
			error(1);//关系运算符错误
		}
	}
	else
	{
		error(1);//缺少while
	}

}
void ifsentence()
{
	getSym();
	expression();
	if (symbol.type == _equal || symbol.type == _less || symbol.type == _lessequal || symbol.type == _lessmore || symbol.type == _more || symbol.type == _moreequal)
	{
		getSym();
		expression();
		if (symbol.type == _then)
		{
			getSym();
			sentence();
			if (symbol.type == _else)
			{
				getSym();
				sentence();
			} 
		}
		else
		{
			error(1);//缺少then
		}
	}
	else
	{
		error(1);//比较运算符错误
	}
}
void sentence()
{
	if (symbol.type == _if)
	{
		ifsentence(); 
	}
	else if (symbol.type == _do)
	{
		dowhile();
	}
	else if (symbol.type == _begin)
	{
		compound();
	}
	else if (symbol.type == _write)
	{
		write();
	}
	else if (symbol.type == _read)
	{
		read();
	}
	else if (symbol.type == _for)
	{
		forloop();
	}
	else if(symbol.type == _identifier)
	{
		symItem sym = findSymTable(symbol.identifier);
		if (sym.name.empty())
		{
			error(1);//未找到该标识符
		}
		else
		{
			if (sym.type == _integer || sym.type == _char || sym.type == _function)
			{
				assignment();
			}
			else
			{
				callprocedure();
			}
			
		}
	}
}
void callprocedure()
{
	getSym();
	if (symbol.type == _lparenthese)
	{
		do 
		{
			getSym();
			expression();
		} while (symbol.type == _comma);
		if (symbol.type != _rparenthese)
		{
			error(1);//缺少)
		}
		getSym();
	}
}
void assignment()
{
	getSym();
	if (symbol.type == _assign)
	{
		getSym();
		expression();

	} 
	else if (symbol.type == _lbracket)
	{
		getSym();
		expression();

		if (symbol.type == _rbracket)
		{
			getSym();
			if (symbol.type == _assign)
			{
				getSym();
				expression();

			}
		}
		else
		{
			error(1);//缺少]
		}
	}
	else
	{
		error(1);//缺少:=
	}
}