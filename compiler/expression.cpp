#include "global.h"
operand factor()
{
	if (symbol.type == _identifier)
	{
		symItem sym = findSymTable(symbol.identifier);
		if (sym.type == _function)
		{
			operand rtn = { 0,0,symbol.identifier };
			callfunction();
			return rtn;
		} 
		else if (sym.type == _integer || sym.type == _char)
		{
			operand rtn = { 0,0,symbol.identifier };
			getSym();
			if (symbol.type == _lbracket)
			{
				getSym();
				operand rtn2 = expression();
				if (symbol.type == _rbracket)
				{
					getSym();
					operand temp = alloc_temp();
					gen_icode(q_array, rtn, rtn2, temp);
					return temp;
				}
				else
				{
					error(16);//缺少]
				}
			}
			return rtn;
		}
		else
		{
			error(1);//非法标识符
			return{};
		}
	
	}
	else if (symbol.type == _constant)
	{
		operand rtn = { 1,symbol.value };
		getSym();
		return rtn;
	}
	else if (symbol.type == _lparenthese)
	{
		operand rtn = expression();
		getSym();
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		return rtn;
	}
}
operand term()
{
	operand f1 = factor();
	int mark = 0;
	operand temp = alloc_temp();
	while (symbol.type == _multi || symbol.type == _div)
	{
		getSym();
		if (mark)
		{
			operand f2 = factor();
			operand temp2 = alloc_temp();
			gen_icode(q_mul, f1, temp, temp2);
			temp.name = temp2.name;
		} 
		else
		{
			operand f2 = factor();
			gen_icode(q_mul, f1, f2, temp);
		}
		mark = 1;
		
	}
	return temp;
}
operand expression()
{
	int mark = 0;
	operand temp0 = { 1,0 };
	do 
	{
		if (symbol.type == _plus || symbol.type == _sub)
		{
			mark = symbol.type;
			getSym();
		}
		operand t1 = term();
		if (mark)
		{
			operand temp = alloc_temp();
			if (mark == _plus)
			{
				gen_icode(q_add, temp0, t1, temp);
				temp0 = temp;
			} 
			else
			{
				gen_icode(q_sub, temp0, t1, temp);
				temp0 = temp;
			}
		}
		else
		{
			temp0 = t1;
		}
	}while (symbol.type == _plus || symbol.type == _sub);
	return temp0;
}
void callfunction()
{
	string funcname = symbol.identifier;
	getSym();
	if (symbol.type == _lparenthese)
	{
		do
		{
			getSym();
			operand param = expression();
			gen_icode(q_push, {}, {}, param);
		} while (symbol.type == _comma);
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		gen_icode(q_call, {}, {}, { 0,0,funcname });
		getSym();
	}
}