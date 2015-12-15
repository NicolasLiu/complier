#include "global.h"
operand factor()
{
	if (symbol.type == _identifier)
	{
		symItem sym = findSymTable(symbol.identifier);
		if (sym.type == _function)
		{
			operand rtn = { _function,sym.value,0,symbol.identifier };
			callfunction();
			return rtn;
		} 
		else if (sym.type == _integer || sym.type == _char)
		{
			operand rtn;
			if (sym.constvar == _const)
			{
				rtn = { _constant,sym.value,0,"",0,0,sym.type };
			} 
			else
			{
				rtn = { sym.type,0,0,symbol.identifier };
			}
			
			getSym();
			if (symbol.type == _lbracket)
			{
				getSym();
				operand rtn2 = expression();
				if (symbol.type == _rbracket)
				{
					getSym();
					if (sym.dimension != 1)
					{
						error(46);//非法的数组
					}
					operand temp = alloc_temp(sym.type);
					gen_icode(q_array, rtn, rtn2, temp);
					return temp;
				}
				else
				{
					error(16);//缺少]
				}
			}
			if (sym.dimension != 0)
			{
				error(45);//数组类型缺少[]
			}
			return rtn;
		}
		else if (sym.name.empty())
		{
			error(41);//未找到该标识符
			return{};
		}
		else
		{
			error(29);//非法的标识符
			return{};
		}
	
	}
	else if (symbol.type == _constant)
	{
		operand rtn = { _constant,symbol.value };
		getSym();
		return rtn;
	}
	else if (symbol.type == _lparenthese)
	{
		getSym();
		operand rtn = expression();
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		getSym();
		return rtn;
	}
	else
	{
		error(29);//非法的标识符
		return{};
	}
	
}
operand term()
{
	operand f1 = factor();
	int mark = 0;
	operand temp = f1;
	while (symbol.type == _multi || symbol.type == _div)
	{
		int op = symbol.type - 22;
		getSym();
		if (mark)
		{
			operand f2 = factor();
			operand temp2;
			if (temp.type == _integer || f2.type == _integer || (temp.type == _function&&temp.value == _integer) || (f2.type == _function&&f2.value == _integer))
			{
				temp2 = alloc_temp(_integer);
			}
			else
			{
				temp2 = alloc_temp(_char);
			}
			gen_icode(op, f2, temp, temp2);
			temp = temp2;
		} 
		else
		{
			operand f2 = factor();
			if (f1.type == _integer || f2.type == _integer || (f1.type == _function&&f1.value == _integer) || (f2.type == _function&&f2.value == _integer))
			{
				temp = alloc_temp(_integer);
			}
			else
			{
				temp = alloc_temp(_char);
			}
			
			gen_icode(op, f1, f2, temp);
		}
		mark = 1;
		
	}
	return temp;
}
operand expression()
{
	int mark = 0;
	operand temp0 = { _constant,0 };
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
			operand temp;
			if (temp0.type == _integer || t1.type == _integer || (temp0.type == _function&&temp0.value == _integer) || (t1.type == _function&&t1.value == _integer))
			{
				temp = alloc_temp(_integer);
			}
			else
			{
				temp = alloc_temp(_char);
			}
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
	int paramNum = 0;
	symItem psym = findSymTable(symbol.identifier);
	getSym();
	if (symbol.type == _lparenthese)
	{
		do
		{
			getSym();
			operand param = expression();
			paramNum++;
			if (paramNum > psym.paramnum)
			{
				error(44);//参数类型不一致
			}
			else
			{
				int temp_type = param.type == _constant ? _integer : param.type;
				if (temp_type != psym.params[paramNum - 1][1])
				{
					error(44);//参数类型不一致
				}
				else if (psym.params[paramNum - 1][0] == 1 && param.name[0] == '_')
				{
					error(43);//var类型参数应对应变量
				}
			}
			param.isvar = psym.params[paramNum - 1][0];
			gen_icode(q_push, {}, {}, param);
		} while (symbol.type == _comma);
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		gen_icode(q_call, {}, {}, { _function,psym.value,0,funcname });
		getSym();
	}
}