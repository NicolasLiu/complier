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
				gen_icode(q_param, {}, {}, { 0,0,symbol.identifier });
				getSym();
			}
			else
			{
				error(29);//非法的标识符
			}
		}
		while (symbol.type == _comma);
		if (symbol.type == _rparenthese)
		{
			getSym();
			gen_icode(q_call, {}, {}, { 0,0,"read" });
			return;
		}
		else
		{
			error(11);//缺少;
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
			gen_icode(q_param, {}, {}, { 2,0,symbol.identifier });
			getSym();
			if (symbol.type == _comma)
			{
				getSym();
			}
			else
			{
				if (symbol.type == _rparenthese)
				{
					gen_icode(q_call, {}, {}, { 0,0,"write" });
					return;
				}
				else
				{
					error(12);//缺少)
				}
			}
		}
		operand exp1 = expression();
		if (symbol.type == _rparenthese)
		{
			gen_icode(q_param, {}, {}, exp1);
			gen_icode(q_call, {}, {}, { 0,0,"write" });
			getSym();
			return;
		}
		else
		{
			error(12);//缺少)
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
		error(21);//缺少end
	}
}
void forloop()
{
	getSym();
	if (symbol.type == _identifier)
	{
		operand i = { 0,0,symbol.identifier };
		getSym();
		if (symbol.type == _assign)
		{
			getSym();
			operand i_init = expression();
			gen_icode(q_mov, i, i_init, {});
			operand label = alloc_label();
			gen_icode(q_label, {}, {}, label);
			if (symbol.type == _to || symbol.type == _downto)
			{
				int mark = symbol.type;
				getSym();
				operand i_end = expression();
				if (symbol.type == _do)
				{
					getSym();
					sentence();
					if (mark == _to)
					{
						gen_icode(q_accumulate, i, { 1,1 }, {});
						gen_icode(q_jle, i, i_end, label);
					} 
					else
					{
						gen_icode(q_accumulate, i, { 1,-1 }, {});
						gen_icode(q_jge, i, i_end, label);
					}
					
					return;
				}
				else
				{
					error(20);//缺少do
				}
			}
			else
			{
				error(19);//缺少to或downto
			}
		}
		else
		{
			error(15);//缺少:=
		}
	}
	else
	{
		error(29);//非法的标识符
	}
}
void dowhile()
{
	operand label = alloc_label();
	gen_icode(q_label, {}, {}, label);
	sentence();
	getSym();
	if (symbol.type == _while)
	{
		getSym();
		operand arg1 = expression();
		if (symbol.type == _equal || symbol.type == _less || symbol.type == _lessequal || symbol.type == _lessmore || symbol.type == _more || symbol.type == _moreequal)
		{
			getSym();
			operand arg2 = expression();
			gen_icode(symbol.type - 30, arg1, arg2, label);
			return;
		}
		else
		{
			error(31);//非法的关系运算符
		}
	}
	else
	{
		error(18);//缺少while
	}

}
void ifsentence()
{
	getSym();
	operand arg1 = expression();
	if (symbol.type == _equal || symbol.type == _less || symbol.type == _lessequal || symbol.type == _lessmore || symbol.type == _more || symbol.type == _moreequal)
	{
		int optype = symbol.type;
		getSym();
		operand arg2 = expression();
		operand label = alloc_label();
		gen_icode(optype - 30, {}, {}, label);
		if (symbol.type == _then)
		{
			getSym();
			sentence();
			gen_icode(q_label, arg1, arg2, label);
			if (symbol.type == _else)
			{
				getSym();
				sentence();
			} 
		}
		else
		{
			error(17);//缺少then
		}
	}
	else
	{
		error(24);//非法的比较运算符
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
	operand procedure = { 0,0,symbol.identifier };
	getSym();
	if (symbol.type == _lparenthese)
	{
		do 
		{
			getSym();
			operand p = expression();
			gen_icode(q_param, {}, {}, p);
		} while (symbol.type == _comma);
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		gen_icode(q_call, {}, {}, procedure);
		getSym();
	}
}
void assignment()
{
	operand dst = { 0,0,symbol.identifier };
	getSym();
	if (symbol.type == _assign)
	{
		getSym();
		operand src = expression();
		gen_icode(q_mov, dst, src, {});
	} 
	else if (symbol.type == _lbracket)
	{
		getSym();
		operand dst2 = expression();
		if (symbol.type == _rbracket)
		{
			operand temp = alloc_temp();
			gen_icode(q_array, dst, dst2, temp);
			getSym();
			if (symbol.type == _assign)
			{
				getSym();
				operand src = expression();
				gen_icode(q_mov, temp, src, {});
			}
		}
		else
		{
			error(16);//缺少]
		}
	}
	else
	{
		error(15);//缺少:=
	}
}