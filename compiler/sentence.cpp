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
				error(1);//�Ƿ��ı�ʶ��
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
			error(1);//Ӧ�ԷֺŽ�β
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
					error(1);//д���ȱ��������
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
			error(1);//д���ȱ��������
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
		error(1);//ȱ��end
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
					error(1);//ȱ��do
				}
			}
			else
			{
				error(1);//ȱ��to��downto
			}
		}
		else
		{
			error(1);//ȱ��:=
		}
	}
	else
	{
		error(1);//�Ƿ��ı�ʶ��
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
			error(1);//��ϵ���������
		}
	}
	else
	{
		error(1);//ȱ��while
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
			error(1);//ȱ��then
		}
	}
	else
	{
		error(1);//�Ƚ����������
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
			error(1);//δ�ҵ��ñ�ʶ��
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
			error(1);//ȱ��)
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
			error(1);//ȱ��]
		}
	}
	else
	{
		error(1);//ȱ��:=
	}
}