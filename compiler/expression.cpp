#include "global.h"
void factor()
{
	if (symbol.type == _identifier)
	{
		symItem sym = findSymTable(symbol.identifier);
		if (sym.type == _function)
		{
			callfunction();
		} 
		else if (sym.type == _integer || sym.type == _char)
		{
			getSym();
			if (symbol.type == _lbracket)
			{
				getSym();
				expression();
				if (symbol.type == _rbracket)
				{
					getSym();
					return;
				}
				else
				{
					error(1);//缺少]
				}
			}
		}
		else
		{
			error(1);//非法标识符
		}
	
	}
	else if (symbol.type == _constant)
	{
		getSym();
		return;
	}
	else if (symbol.type == _lparenthese)
	{
		expression();
		getSym();
		if (symbol.type != _rparenthese)
		{
			error(1);//缺少)
		}
		
	}
}
void term()
{
	factor();

	while (symbol.type == _multi || symbol.type == _div)
	{
		getSym();
		factor();
	}
}
void expression()
{
	do 
	{
		if (symbol.type == _plus || symbol.type == _sub)
		{
			getSym();
		}
		term();
	}while (symbol.type == _plus || symbol.type == _sub);
}
void callfunction()
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