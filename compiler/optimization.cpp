#include "global.h"
typedef struct _dagnode
{
	int type;
	operand result;
	int ref;
	int op1;
	int op2;
} dagNode;
typedef struct _block
{
	vector<quaternion> content;
	set<string> bin, bout, buse, bdef;
	int nextnum;
	int next1;
	int next2;

}basicblock;
typedef struct _chongtuNode
{
	string name;
	set<int> other;
	int reg;
	int rm;
}ctNode;
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
		if (type == q_add || type == q_mul)
		{
			if (iter->type == type && ((iter->op1 == op1 && iter->op2 == op2) || (iter->op1 == op2 && iter->op2 == op1)))
			{
				return iter - dagMap.begin();
			}
		} 
		else
		{
			if (iter->type == type && iter->op1 == op1 && iter->op2 == op2)
			{
				return iter - dagMap.begin();
			}
		}
		
		iter++;
	}
	return -1;
}
void DAG(vector<quaternion>::iterator &iter, vector<quaternion> &newList)
{
	vector<dagNode> dagMap;
	unordered_map<string,int> dagTable;
	stack<int> stack1,pushstack;
	while (true)
	{
		if (iter->op == q_end || iter->op == q_j || iter->op == q_jne || iter->op == q_jl || iter->op == q_jle || iter->op == q_jg || iter->op == q_jge || iter->op == q_je || iter->op == q_label || (iter->op == q_call && (iter->answer.name.compare("read") != 0 && iter->answer.name.compare("write") != 0)) || iter->op == q_accumulate)
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
				else if (dagMap[temp].type == q_push || dagMap[temp].type == q_call)
				{
					newList.push_back({ dagMap[temp].type, {},{},dagMap[temp].result });
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
			else if (iter->op == q_push)
			{
				dagMap.push_back({ q_push,iter->answer,0,-1,-1 });
				pushstack.push(dagMap.size() - 1);
			}
			else if (iter->op == q_call)
			{
				if (iter->answer.name.compare("read") == 0)
				{
					while (!pushstack.empty())
					{
						int pos = pushstack.top();
						unordered_map<string, int>::iterator iterarg1 = dagTable.find(dagMap[pos].result.name);
						pushstack.pop();
						if (iterarg1 != dagTable.end())
						{
							iterarg1->second = pos;
						}
						
					}
					dagMap.push_back({ q_call,iter->answer,0,-1,-1 });
				}
				else
				{
					dagMap.push_back({ q_call,iter->answer,0,-1,-1 });
					while (!pushstack.empty())
					{
						pushstack.pop();
					}
					
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
int findctNode(vector<ctNode> map, string tofind)
{
	for (vector<ctNode>::iterator iter = map.begin(); iter != map.end(); iter++)
	{
		if (tofind.compare(iter->name) == 0)
		{
			return iter - map.begin();
		}
	}
	return -1;
}
int rmctNode(vector<ctNode> &map)
{
	for (vector<ctNode>::iterator iter = map.begin(); iter != map.end(); iter++)
	{
		if (!iter->name.empty() && iter->other.size() < 3 && iter->rm == 0)
		{
			iter->rm = 1;
			for (set<int>::iterator iter2 = iter->other.begin(); iter2 != iter->other.end(); iter2++)
			{
				map[*iter2].other.erase(map[*iter2].other.find(iter - map.begin()));
			}
			iter->other.clear();
			return iter - map.begin();
		}
	}
	return -1;
}
void rmctNode2(vector<ctNode> &map)
{
	for (vector<ctNode>::iterator iter = map.begin(); iter != map.end(); iter++)
	{
		if (!iter->name.empty() && iter->other.size() >= 3 && iter->rm == 0)
		{
			iter->rm = 1;
			for (set<int>::iterator iter2 = iter->other.begin(); iter2 != iter->other.end(); iter2++)
			{
				map[*iter2].other.erase(map[*iter2].other.find(iter - map.begin()));
			}
			iter->other.clear();
			return;
		}
	}
}
void allocReg(vector<ctNode> &map, int pos)
{
	int status[3] = { 0,0,0 };
	int toalloc[3] = { 2,4,5 };
	for (set<int>::iterator iter = map[pos].other.begin(); iter != map[pos].other.end(); iter++)
	{
		if (map[*iter].reg == 2)
		{
			status[0] = 1;
		}
		else if (map[*iter].reg == 4)
		{
			status[1] = 1;
		}
		else if(map[*iter].reg == 5)
		{
			status[2] = 1;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (status[i] == 0)
		{
			map[pos].reg = toalloc[i];
			return;
		}
	}
	
}
void LiveVarAnalysis(vector<quaternion>::iterator &iter)
{
	vector<quaternion>::iterator saveiter = iter;
	vector<basicblock> blocks;
	unordered_map<string, int> labelindex;
	blocks.push_back({ {} });
	while (iter->op != q_end)
	{
		if (iter->op == q_j || iter->op == q_jl || iter->op == q_jle || iter->op == q_jg || iter->op == q_jge || iter->op == q_je || iter->op == q_jne)
		{
			blocks.back().content.push_back(*iter);
			blocks.push_back({ {},{},{},{},{} });
		}
		else if (iter->op == q_label)
		{
			if (blocks.back().content.empty() || blocks.back().content.back().op == q_label)
			{
				blocks.back().content.push_back(*iter);
			}
			else
			{
				blocks.push_back({ {},{},{},{},{} });
				blocks.back().content.push_back(*iter);
			}
			labelindex.insert({ iter->answer.name, blocks.size() - 1 });
		}
		else
		{
			blocks.back().content.push_back(*iter);
		}
		iter++;
	}
	if (blocks.back().content.empty())
	{
		blocks.pop_back();
	}
	for (vector<basicblock>::iterator iter2 = blocks.begin(); iter2 != blocks.end(); iter2++)
	{
		int lastop = iter2->content.back().op;
		if (lastop == q_jl || lastop == q_jle || lastop == q_jg || lastop == q_jge || lastop == q_je || lastop == q_jne)
		{
			iter2->nextnum = 2;
			iter2->next1 = labelindex.find(iter2->content.back().answer.name)->second;
			iter2->next2 = iter2 - blocks.begin() + 1;
			if (iter2->next2 > blocks.size() - 1)
			{
				iter2->next2 = 0;
				iter2->nextnum = 1;
			}
		}
		else if (lastop == q_j)
		{
			iter2->nextnum = 1;
			iter2->next1 = labelindex.find(iter2->content.back().answer.name)->second;
		}
		else
		{
			iter2->nextnum = 1;
			iter2->next1 = iter2 - blocks.begin() + 1;
			if (iter2->next1 > blocks.size() - 1)
			{
				iter2->next1 = 0;
				iter2->nextnum = 0;
			}
		}

		for (vector<quaternion>::iterator iter3 = iter2->content.begin(); iter3 != iter2->content.end(); iter3++)
		{
			if (iter3->op == q_mov)
			{
				if (!iter3->arg1.name.empty() && iter3->arg1.name[0] != '_')
				{
					iter2->bdef.insert(iter3->arg1.name);
				}
				if (!iter3->arg2.name.empty() && iter3->arg2.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg2.name);
				}
			}
			else if (iter3->op == q_add || iter3->op == q_sub || iter3->op == q_mul || iter3->op == q_div || iter3->op == q_array)
			{
				if (!iter3->arg1.name.empty() && iter3->arg1.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg1.name);
				}
				if (!iter3->arg2.name.empty() && iter3->arg2.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg2.name);
				}
				if (!iter3->answer.name.empty() && iter3->answer.name[0] != '_')
				{
					iter2->bdef.insert(iter3->answer.name);
				}
			}
			else if (iter3->op == q_jl || iter3->op == q_jle || iter3->op == q_jg || iter3->op == q_jge || iter3->op == q_je || iter3->op == q_jne)
			{
				if (!iter3->arg1.name.empty() && iter3->arg1.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg1.name);
				}
				if (!iter3->arg2.name.empty() && iter3->arg2.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg2.name);
				}
			}
			else if (iter3->op == q_push || iter3->op == q_accumulate)
			{
				if (!iter3->arg1.name.empty() && iter3->arg1.name[0] != '_')
				{
					iter2->buse.insert(iter3->arg1.name);
				}
			}
		}
	}
	int mark = 3;
	do 
	{
		for (vector<basicblock>::reverse_iterator iter2 = blocks.rbegin(); iter2 != blocks.rend(); iter2++)
		{
			if (iter2->nextnum == 2)
			{
				iter2->bout.clear();
				set_union(blocks[iter2->next1].bin.begin(), blocks[iter2->next1].bin.end(), blocks[iter2->next2].bin.begin(), blocks[iter2->next2].bin.end(), inserter(iter2->bout,iter2->bout.begin()));
			}
			else if (iter2->nextnum == 1)
			{
				iter2->bout.clear();
				set<string> temp(blocks[iter2->next1].bin);
				iter2->bout = temp;
			}

			set<string> temp, temp2;
			set_difference(iter2->bout.begin(), iter2->bout.end(), iter2->bdef.begin(), iter2->bdef.end(), inserter(temp, temp.begin()));
			set_union(iter2->buse.begin(), iter2->buse.end(), temp.begin(), temp.end(), inserter(temp2, temp2.begin()));
			iter2->bin = temp2;
		}
	} while (mark--);

	vector<ctNode> ctMap;
	for (vector<basicblock>::iterator iter2 = blocks.begin(); iter2 != blocks.end(); iter2++)
	{
		int num = iter2->bin.size();
		vector<int> nowmap;
		for (set<string>::iterator iter3 = iter2->bin.begin(); iter3 != iter2->bin.end(); iter3++)
		{
			int pos = findctNode(ctMap, *iter3);
			if (pos == -1)
			{
				ctMap.push_back({ *iter3,{} });
				nowmap.push_back(ctMap.size() - 1);
			} 
			else
			{
				nowmap.push_back(pos);
			}
		}
		for (int i = 0; i < num; i++)
		{
			for (int j = i + 1; j < num; j++)
			{
				ctMap[i].other.insert(j);
				ctMap[j].other.insert(i);
			}
		}

	}
	vector<ctNode> ctMap2(ctMap);
	stack<int> rmstack;
	for (int i = 0; i < ctMap2.size(); i++)
	{
		int pos = rmctNode(ctMap);
		if (pos != -1)
		{
			rmstack.push(pos);
		}
		else
		{
			rmctNode2(ctMap);
		}
	}
	while (!rmstack.empty())
	{
		int pos = rmstack.top();
		allocReg(ctMap2,pos);
		rmstack.pop();
	}
	while (saveiter->op != q_end)
	{
		int pos = findctNode(ctMap2, saveiter->arg1.name);
		if (pos != -1)
		{
			saveiter->arg1.reg = ctMap2[pos].reg;
		} 
		pos = findctNode(ctMap2, saveiter->arg2.name);
		if (pos != -1)
		{
			saveiter->arg2.reg = ctMap2[pos].reg;
		}
		pos = findctNode(ctMap2, saveiter->answer.name);
		if (pos != -1)
		{
			saveiter->answer.reg = ctMap2[pos].reg;
		}
		saveiter++;
	}
}
void GlobalRegisterAllocate()
{
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		if (iter->op == q_begin)
		{
			iter++;
			LiveVarAnalysis(iter);
		}
		iter++;
	}
}
void optimization(int open)
{
	ConstantFolding();
	if (open)
	{
		CommonSubexpressionElimination();
		//GlobalRegisterAllocate();
	}
	
}