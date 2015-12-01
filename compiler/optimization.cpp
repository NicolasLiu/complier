#include "global.h"

void calc_constant()//消除常数运算
{
	unordered_map<string, int> update;
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		if (!iter->arg1.name.empty())
		{
			unordered_map<string, int>::iterator iter_update = update.find(iter->arg1.name);
			if (iter_update != update.end())
			{
				iter->arg1 = { _constant,iter_update->second };
			}
		}
		if (!iter->arg2.name.empty())
		{
			unordered_map<string, int>::iterator iter_update = update.find(iter->arg2.name);
			if (iter_update != update.end())
			{
				iter->arg2 = { _constant,iter_update->second };
			}
		}
		if (iter->op == q_add || iter->op == q_sub || iter->op == q_mul || iter->op == q_div)
		{
			operand arg1 = iter->arg1, arg2 = iter->arg2;
			if (arg1.type = _constant && arg2.type == _constant)
			{
				int result = 0;
				string resultvar = iter->answer.name;
				switch (iter->op)
				{
				case q_add:
					result = arg1.value + arg2.value;
					break;
				case q_sub:
					result = arg1.value - arg2.value;
					break;
				case q_mul:
					result = arg1.value * arg2.value;
					break;
				case q_div:
					result = arg1.value / arg2.value;
					break;
				}
				iter = quaternionList.erase(iter);
				update.insert({ resultvar, result });
				continue;
			}
		}

		iter++;
	}
}
void optimization()
{
	calc_constant();
}