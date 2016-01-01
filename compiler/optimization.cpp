#include "global.h"
typedef struct _dagnode
{
	int type;
	operand result;
	int ref;
	int op1;
	int op2;
} dagNode;
void ConstantFolding()//消除常数运算
{
	unordered_map<string, int> update;
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		if (iter->op == q_jne || iter->op == q_jge || iter->op == q_jg || iter->op == q_je || iter->op == q_jle || iter->op == q_jl)
		{
			operand arg1 = iter->arg1, arg2 = iter->arg2;
			if (arg1.type == _constant && arg2.type == _constant)
			{
				int mark = 0;
				switch (iter->op)
				{
				case q_jne:
					mark = arg1.value != arg2.value;
					break;
				case q_jge:
					mark = arg1.value >= arg2.value;
					break;
				case q_jg:
					mark = arg1.value > arg2.value;
					break;
				case q_je:
					mark = arg1.value == arg2.value;
					break;
				case q_jle:
					mark = arg1.value <= arg2.value;
					break;
				case q_jl:
					mark = arg1.value < arg2.value;
					break;
				}
				if (mark)
				{
					iter->op = q_j;
				}
			}
		}
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
			if (arg1.type == _constant && arg2.type == _constant)
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
int findDAGNode(vector<dagNode> dagMap, int type, int op1, int op2)
{
	vector<dagNode>::iterator iter = dagMap.begin();
	while (iter != dagMap.end())
	{
		if (iter->type == type && iter->op1 == op1 && iter->op2 == op2)
		{
			return iter - dagMap.begin();
		}
		iter++;
	}
	return -1;
}
void DAG(vector<quaternion>::iterator &iter, vector<quaternion> &newList)
{
	vector<dagNode> dagMap;
	unordered_map<string,int> dagTable;
	stack<int> stack1;
	while (true)
	{
		if (iter->op == q_end || iter->op == q_j || iter->op == q_jne || iter->op == q_jl || iter->op == q_jle || iter->op == q_jg || iter->op == q_jge || iter->op == q_je || iter->op == q_label || iter->op == q_call || iter->op == q_push || iter->op == q_accumulate)
		{
			for (vector<dagNode>::reverse_iterator iterdag = dagMap.rbegin(); iterdag != dagMap.rend(); iterdag++)
			{
				if (iterdag->type != 0)
				{
					stack1.push(dagMap.rend() - iterdag - 1);
				}
			}
			while (!stack1.empty())
			{
				int temp = stack1.top();
				stack1.pop();
				if (dagMap[temp].type == q_add || dagMap[temp].type == q_sub || dagMap[temp].type == q_mul || dagMap[temp].type == q_div || dagMap[temp].type == q_array)
				{
					newList.push_back({ dagMap[temp].type, dagMap[dagMap[temp].op1].result,dagMap[dagMap[temp].op2].result ,dagMap[temp].result });
				} 
				else
				{
					newList.push_back({ dagMap[temp].type, dagMap[temp].result,dagMap[dagMap[temp].op1].result });
				}
				
			}
			if (iter->op != q_end)
			{
				newList.push_back(*iter);
				iter++;
			}
			return;
		}
		else
		{
			if (iter->op == q_add || iter->op == q_sub || iter->op == q_mul || iter->op == q_div)
			{
				int op1, op2, answer;
				//操作数1
				string op1name;
				if (iter->arg1.type == _constant)
				{
					stringstream ss;
					ss << iter->arg1.value;
					op1name = ss.str();
				} 
				else
				{
					op1name = iter->arg1.name;
				}
				unordered_map<string, int>::iterator iterarg1 = dagTable.find(op1name);
				if (iterarg1 == dagTable.end())
				{
					dagMap.push_back({ 0,iter->arg1,iter->arg1.type != _constant,-1,-1 });
					op1 = dagMap.size() - 1;
					dagTable.insert({ op1name ,op1 });
				}
				else
				{
					op1 = iterarg1->second;
				}
				//操作数2
				string op2name;
				if (iter->arg2.type == _constant)
				{
					stringstream ss;
					ss << iter->arg2.value;
					op2name = ss.str();
				}
				else
				{
					op2name = iter->arg2.name;
				}
				unordered_map<string, int>::iterator iterarg2 = dagTable.find(op2name);
				if (iterarg2 == dagTable.end())
				{
					dagMap.push_back({ 0,iter->arg2,iter->arg2.type != _constant,-1,-1 });
					op2 = dagMap.size() - 1;
					dagTable.insert({ op2name ,op2 });
				}
				else
				{
					op2 = iterarg2->second;
				}
				//操作数3
				answer = findDAGNode(dagMap, iter->op, op1, op2);
				if (answer == -1)
				{
					dagMap.push_back({ iter->op,iter->answer,0,op1,op2 });
					answer = dagMap.size() - 1;
					dagTable.insert({ iter->answer.name ,answer });
				} 
				else
				{
					dagTable.insert({ iter->answer.name ,answer });
				}
				
			}
			else if (iter->op == q_array)
			{
				int op1, op2, answer;
				//操作数1
				string op1name;
				if (iter->arg1.type == _constant)
				{
					stringstream ss;
					ss << iter->arg1.value;
					op1name = ss.str();
				}
				else
				{
					op1name = iter->arg1.name;
				}
				unordered_map<string, int>::iterator iterarg1 = dagTable.find(op1name);
				if (iterarg1 == dagTable.end())
				{
					dagMap.push_back({ 0,iter->arg1,iter->arg1.type != _constant,-1,-1 });
					op1 = dagMap.size() - 1;
					dagTable.insert({ op1name ,op1 });
				}
				else
				{
					op1 = iterarg1->second;
				}
				//操作数2
				string op2name;
				if (iter->arg2.type == _constant)
				{
					stringstream ss;
					ss << iter->arg2.value;
					op2name = ss.str();
				}
				else
				{
					op2name = iter->arg2.name;
				}
				unordered_map<string, int>::iterator iterarg2 = dagTable.find(op2name);
				if (iterarg2 == dagTable.end())
				{
					dagMap.push_back({ 0,iter->arg2,iter->arg2.type != _constant,-1,-1 });
					op2 = dagMap.size() - 1;
					dagTable.insert({ op2name ,op2 });
				}
				else
				{
					op2 = iterarg2->second;
				}
				//操作数3
				dagMap.push_back({ iter->op,iter->answer,0,op1,op2 });
				answer = dagMap.size() - 1;
				dagTable.insert({ iter->answer.name ,answer });
			}
			else if (iter->op == q_mov)
			{
				int op1, op2;
				//操作数2
				string op2name;
				if (iter->arg2.type == _constant)
				{
					stringstream ss;
					ss << iter->arg2.value;
					op2name = ss.str();
				}
				else
				{
					op2name = iter->arg2.name;
				}
				unordered_map<string, int>::iterator iterarg2 = dagTable.find(op2name);
				if (iterarg2 == dagTable.end())
				{
					dagMap.push_back({ 0,iter->arg2,iter->arg2.type != _constant,-1,-1 });
					op2 = dagMap.size() - 1;
					dagTable.insert({ op2name ,op2 });
				}
				else
				{
					op2 = iterarg2->second;
				}
				//操作数1
				unordered_map<string, int>::iterator iterarg1 = dagTable.find(iter->arg1.name);
				if (iterarg1 == dagTable.end())
				{
					dagMap.push_back({ iter->op,iter->arg1,0,op2,-1 });
					op1 = dagMap.size() - 1;
					dagTable.insert({ iter->arg1.name ,op1 });
				}
				else
				{
					dagMap.push_back({ iter->op,iter->arg1,0,op2,-1 });
					op1 = dagMap.size() - 1;
					iterarg1->second = op1;
				}
			}
		}
		iter++;
	}
	
}
void CommonSubexpressionElimination()
{
	vector<quaternion> newList;
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		if (iter->op == q_begin)
		{
			newList.push_back(*iter);
			iter++;
			while (iter->op != q_end)
			{
				DAG(iter, newList);
			}
			newList.push_back(*iter);
		}
		else
		{
			newList.push_back(*iter);
		}
		iter++;
	}
	quaternionList = newList;
}
void optimization()
{
	ConstantFolding();
	CommonSubexpressionElimination();
}