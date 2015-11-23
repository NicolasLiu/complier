#include "global.h"
list<quaternion> quaternionList;
operand alloc_temp()
{
	stringstream ss;
	ss << "_t" << temp_var_t;
	operand rtn = { 0,0,ss.str() };
	temp_var_t++;
	return rtn;
}
void gen_icode(int op, operand arg1, operand arg2, operand answer)
{
	quaternionList.push_back({ op,arg1,arg2,answer });
}