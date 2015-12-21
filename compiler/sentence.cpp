#include "global.h"
unordered_set<string> cannotassign;
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
				symItem sym = findSymTable(symbol.identifier);
				if (sym.name.empty())
				{
					error(41);//未找到该标识符
				}
				else if (sym.type != _integer && sym.type != _char)
				{
					error(44);//参数类型不一致
				}
				else if (sym.dimension != 0)
				{
					error(44);//参数类型不一致
				}
				gen_icode(q_push, {}, {}, { sym.type,0,0,symbol.identifier });
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
			gen_icode(q_call, {}, {}, { _procedure,0,0,"read" });
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
	operand op1 = {};
	if (symbol.type == _lparenthese)
	{
		getSym();
		if (symbol.type == _string)
		{
			op1 = { _string,0,0,symbol.identifier };
			getSym();
			if (symbol.type == _comma)
			{
				getSym();
			}
			else
			{
				if (symbol.type == _rparenthese)
				{
					gen_icode(q_push, {}, {}, op1);
					gen_icode(q_call, {}, {}, { _procedure,0,0,"write" });
					getSym();
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
			if (op1.type != 0)
			{
				gen_icode(q_push, {}, {}, op1);
			}
			gen_icode(q_push, {}, {}, exp1);
			gen_icode(q_call, {}, {}, { _procedure,0,0,"write" });
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
		symItem sym = findSymTable(symbol.identifier);
		string needtoadd = symbol.identifier;
		if (sym.name.empty())
		{
			error(41);//未找到该标识符
		}
		operand i = { sym.type,0,0,symbol.identifier };
		getSym();
		if (symbol.type == _assign)
		{
			getSym();
			operand i_init = expression();
			gen_icode(q_mov, i, i_init, {});
			operand label1 = alloc_label();
			operand label2 = alloc_label();
			operand label3 = alloc_label();
			operand label4 = alloc_label();
			gen_icode(q_j, {}, {}, label2);
			gen_icode(q_label, {}, {}, label1);
			if (symbol.type == _to || symbol.type == _downto)
			{
				int mark = symbol.type;
				getSym();
				operand i_end = expression();
				if (symbol.type == _do)
				{
					getSym();
					cannotassign.insert(needtoadd);
					sentence();
					cannotassign.erase(needtoadd);
					if (mark == _to)
					{
						gen_icode(q_accumulate, i, { _constant,1 }, {});
						gen_icode(q_jle, i, i_end, label1);
						gen_icode(q_j, {}, {}, label3);
						gen_icode(q_label, {}, {}, label2);
						gen_icode(q_jle, i, i_end, label1);
						gen_icode(q_j, {}, {}, label4);
						gen_icode(q_label, {}, {}, label3);
						gen_icode(q_accumulate, i, { _constant,-1 }, {});
						gen_icode(q_label, {}, {}, label4);
						
					} 
					else
					{
						gen_icode(q_accumulate, i, { _constant,-1 }, {});
						gen_icode(q_jge, i, i_end, label1);
						gen_icode(q_j, {}, {}, label3);
						gen_icode(q_label, {}, {}, label2);
						gen_icode(q_jge, i, i_end, label1);
						gen_icode(q_j, {}, {}, label4);
						gen_icode(q_label, {}, {}, label3);
						gen_icode(q_accumulate, i, { _constant,1 }, {});
						gen_icode(q_label, {}, {}, label4);
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
	getSym();
	sentence();
	if (symbol.type == _while)
	{
		getSym();
		operand arg1 = expression();
		if (symbol.type == _equal || symbol.type == _less || symbol.type == _lessequal || symbol.type == _lessmore || symbol.type == _more || symbol.type == _moreequal)
		{
			int jop = 47 - symbol.type;
			getSym();
			operand arg2 = expression();
			gen_icode(jop, arg1, arg2, label);
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
		gen_icode(optype - 30, arg1, arg2, label);
		if (symbol.type == _then)
		{
			getSym();
			sentence();	
			if (symbol.type == _else)
			{
				operand label2 = alloc_label();
				gen_icode(q_j, {}, {}, label2);
				gen_icode(q_label, {}, {}, label);
				getSym();
				sentence();
				gen_icode(q_label, {}, {}, label2);
			}
			else
			{
				gen_icode(q_label, {}, {}, label);
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
			error(41);//未找到该标识符
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
	int paramNum = 0;
	operand procedure = { _procedure,0,0,symbol.identifier };
	symItem psym = findSymTable(procedure.name);
	queue<operand> opqueue;
	getSym();
	if (symbol.type == _lparenthese)
	{
		do 
		{
			getSym();
			operand p = expression();
			paramNum++;
			if (paramNum > psym.paramnum)
			{
				error(44);//参数类型不一致
			}
			else
			{
				int temp_type = p.type == _constant ? (p.constanttype == 0 ? _integer : p.constanttype) : (p.type == _function ? p.value : p.type);
				if (temp_type != psym.params[paramNum - 1][1])
				{
					error(44);//参数类型不一致
				}
				else if (psym.params[paramNum - 1][0] == 1 && p.type == _constant)
				{
					error(43);//var类型参数应对应变量
				}
			}
			p.isvar = psym.params[paramNum - 1][0];
			opqueue.push(p);
			
		} while (symbol.type == _comma);
		if (symbol.type != _rparenthese)
		{
			error(12);//缺少)
		}
		while (!opqueue.empty())
		{
			gen_icode(q_push, {}, {}, opqueue.front());
			opqueue.pop();
		}
		gen_icode(q_call, {}, {}, procedure);
		getSym();
	}
	else
	{
		gen_icode(q_call, {}, {}, procedure);
	}
}
void assignment()
{
	symItem asym = findSymTable(symbol.identifier);
	if (cannotassign.find(symbol.identifier) != cannotassign.end())
	{
		error(47);//赋值操作错误
	}
	operand dst = { asym.type,0,0,symbol.identifier };
	getSym();
	if (symbol.type == _assign)
	{
		getSym();
		operand src = expression();
		if (src.type == _integer && dst.type == _char)
		{
			error(47);//赋值操作错误
		}
		gen_icode(q_mov, dst, src, {});
	} 
	else if (symbol.type == _lbracket)
	{
		getSym();
		operand dst2 = expression();
		if (symbol.type == _rbracket)
		{
			operand temp = alloc_temp(dst.type);
			gen_icode(q_array, dst, dst2, temp);
			getSym();
			if (symbol.type == _assign)
			{
				getSym();
				operand src = expression();
				if (src.type == _integer && temp.type == _char)
				{
					error(47);//赋值操作错误
				}
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