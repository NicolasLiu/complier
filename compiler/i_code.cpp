#include "global.h"
int temp_var_t;//中间代码临时变量标号
int temp_label;//中间代码label标号
list<quaternion> quaternionList;
string opType[20] = { "+","-","*","/","j","jne","jge","jg","je","jle","jl","push","call","return","mov","procedure","function","label","array" ,"accumulate" };
operand alloc_temp()
{
	stringstream ss;
	ss << "_t" << temp_var_t;
	operand rtn = { 0,0,ss.str() };
	temp_var_t++;
	return rtn;
}
operand alloc_label()
{
	stringstream ss;
	ss << "label_" << temp_label;
	operand rtn = { 0,0,ss.str() };
	temp_label++;
	return rtn;
}
void gen_icode(int op, operand arg1, operand arg2, operand answer)
{
	quaternionList.push_back({ op,arg1,arg2,answer });
}
string print_oprand(operand op)
{
	if (op.type == 0)
	{
		return op.name;
	} 
	else if (op.type == 2)//string
	{
		return op.name;
	}
	else
	{
		stringstream ss;
		ss << op.value;
		return ss.str();
	}
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
			cout << print_oprand(q.answer) << ":";
		}
		else
		{
			cout << "\t<" << opType[q.op - 1] << ",\t" << print_oprand(q.arg1) << ",\t" << print_oprand(q.arg2) << ",\t" << print_oprand(q.answer) << ">" << endl;
		}
		
	}
}
