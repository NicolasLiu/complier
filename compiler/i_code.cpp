#include "global.h"
int temp_var_t;//中间代码临时变量标号
int temp_label;//中间代码label标号
vector<quaternion> quaternionList;
string opType[40] = { "+","-","*","/","j","jne","jge","jg","jle","jl","je","push","call","mov","procedure","function","label","array" ,"accumulate","begin","end","alloc","local" };
operand alloc_temp(int type)
{
	stringstream ss;
	ss << "_t" << temp_var_t;
	operand rtn = { type,0,0,ss.str() };
	temp_var_t++;
	return rtn;
}
operand alloc_label()
{
	stringstream ss;
	ss << "label_" << temp_label;
	operand rtn = { _string,0,0,ss.str() };
	temp_label++;
	return rtn;
}
void gen_icode(int op, operand arg1, operand arg2, operand answer)
{
	if (errorNum)
	{
		return;
	}
	quaternionList.push_back({ op,arg1,arg2,answer });
}
string print_oprand(operand op)
{
	if (op.type == _string || op.type == _function || op.type == _procedure || op.type == _integer || op.type == _char || op.type == _array)
	{
		return op.name;
	} 
	else if(op.type == _constant)
	{
		stringstream ss;
		ss << op.value;
		return ss.str();
	}
	return "";
}
void print_icode()
{
	if (errorNum)
	{
		return;
	}
	for (quaternion q : quaternionList)
	{
		if (q.op == q_function || q.op == q_procedure || q.op == q_label)
		{
			cout << print_oprand(q.answer) << ":" << endl;
		}
		else if (q.op == q_begin)
		{
			cout << "\tbegin" << endl;
		}
		else if (q.op == q_end)
		{
			cout << "\tend" << endl;
		}
		else
		{
			cout << "\t<" << opType[q.op - 1] << ",\t" << print_oprand(q.arg1) << ",\t" << print_oprand(q.arg2) << ",\t" << print_oprand(q.answer) << ">" << endl;
		}
		
	}
	cout << "\t\t四元式行数:" << quaternionList.size() << endl;
}
