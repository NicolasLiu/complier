#include "global.h"

typedef struct _memManageItem
{
	string name;
	int reg;
	int level;
	int offset;
	int isRef;
	int isFunc;
	int paramoffset;
} memManageItem;
typedef struct _asmstackframe
{
	string procName;
	string ss;
	int currentoffset;
} asmStackFrame;
stack<asmStackFrame> asmStack;
int registermark[6] = { 0,0,0,0,0,0 };//EAX,EBX,ECX,EDX,ESI,EDI
char registername[6][4] = { "eax","ebx","ecx","edx","esi","edi" };
string currentProc;
int currentLevel = 0;
list<list<memManageItem>> memManage;
stringstream ss_data, ss_code;
int string_temp_num = 0;
/**************运行时存储分配********************/
int alloc_register()
{
	return -1;
	for (int i = 0; i < 6; i++)
	{
		if (registermark[i] == 0)
		{
			registermark[i] = 1;
			return  i;
		}
	}
	return -1;
}

void free_register(int i)
{
	registermark[i] = 0;
}
memManageItem find_memManage_local(string name)
{
	list<list<memManageItem>>::reverse_iterator iter = memManage.rbegin();
	for (list<memManageItem>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
	{
		if (iter2->name.compare(name) == 0)
		{
			return *iter2;
		}
	}
	return{};
}
memManageItem find_memManage(string name)
{
	for (list<list<memManageItem>>::reverse_iterator iter = memManage.rbegin(); iter != memManage.rend(); iter++)
	{
		for (list<memManageItem>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
		{
			if (iter2->name.compare(name) == 0)
			{
				return *iter2;
			}
		}
	}
	
	return{};
}
int insert_memManage(memManageItem item, int autooffset = 1)
{
	memManageItem temp = find_memManage_local(item.name);
	if (temp.name.empty())
	{
		if (autooffset == 1)
		{
			asmStack.top().currentoffset -= 4;
			item.offset = asmStack.top().currentoffset;
		}
		memManage.back().push_back(item);
		return 1;
	}
	return 0;
}
int insert_memManage_father(memManageItem item, int autooffset = 1)
{
	memManageItem temp = find_memManage_local(item.name);
	if (temp.name.empty())
	{
		if (autooffset == 1)
		{
			asmStack.top().currentoffset -= 4;
			item.offset = asmStack.top().currentoffset;
		}
		list<list<memManageItem>>::reverse_iterator iter = memManage.rbegin();
		iter++;
		iter->push_back(item);
		return 1;
	}
	return 0;
}
void update_memManage(memManageItem newItem)
{
	string name = newItem.name;
	for (list<list<memManageItem>>::reverse_iterator iter = memManage.rbegin(); iter != memManage.rend(); iter++)
	{
		for (list<memManageItem>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
		{
			if (iter2->name.compare(name) == 0)
			{
				iter2->paramoffset = newItem.paramoffset;
				return;
			}
		}
	}
}
void free_memManage(string name)
{
	for (list<list<memManageItem>>::reverse_iterator iter = memManage.rbegin(); iter != memManage.rend(); iter++)
	{
		for (list<memManageItem>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
		{
			if (iter2->name.compare(name) == 0)
			{
				iter->erase(iter2);
				return;
			}
		}
	}
}
/************gen_asm子程序*********************/
void gen_asm_procedure(vector<quaternion>::iterator &iter)
{
	if (iter->op == q_function)
	{
		stringstream ss_proc;
		ss_proc << asmStack.top().ss << "\t" << "push" << " 0" << endl;
		asmStack.top().ss = ss_proc.str();
		memManageItem item = { iter->answer.name,-1,currentLevel,0,0,1 };
		insert_memManage(item);
	}
	
	string name = iter->answer.name;
	stringstream ss_proc;
	currentProc = name;
	currentLevel++;
	ss_proc << currentProc << " proc" << endl;
	ss_proc << "\t" << "push ebp" << endl;
	ss_proc << "\t" << "mov ebp,esp" << endl;
	asmStack.push({ currentProc, ss_proc.str() ,0 });
	if (iter->op == q_function)
	{
		stringstream ss;
		ss << asmStack.top().ss;
		asmStack.top().ss = ss.str();
		asmStack.top().currentoffset -= 4;
	}
	list<memManageItem> list1;
	memManage.push_back(list1);
	if (iter->op == q_procedure)
	{
		insert_memManage_father({ asmStack.top().procName, -1, currentLevel, }, 0);
	}
}
void gen_asm_begin()
{
	if (currentLevel > 1)
	{
		stringstream ss_proc;
		ss_proc << asmStack.top().ss;
		ss_proc << "\t" << "mov esp,ebp" << endl;
		ss_proc << "\t" << "add esp,@need@to@add" << endl;
		ss_proc << "\t" << "push" << " eax" << endl;
		ss_proc << "\t" << "push" << " ebx" << endl;
		ss_proc << "\t" << "push" << " ecx" << endl;
		ss_proc << "\t" << "push" << " edx" << endl;
		ss_proc << "\t" << "push" << " edi" << endl;
		ss_proc << "\t" << "push" << " esi" << endl;
		//asmStack.top().currentoffset -= 4 * 6;
		asmStack.top().ss = ss_proc.str();
	}
	else
	{
		stringstream ss_proc;
		ss_proc << asmStack.top().ss;
		ss_proc << "\t" << "mov esp,ebp" << endl;
		ss_proc << "\t" << "add esp,@need@to@add" << endl;
		asmStack.top().ss = ss_proc.str();
	}
	
}
void gen_asm_end()
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem item = find_memManage(currentProc);
	if (currentLevel > 1)
	{	
		if (item.isFunc)
		{
			ss_proc << "\t" << "mov eax,[ebp-4]" << endl;
			ss_proc << "\t" << "push ebp" << endl;
			ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << 8 << "]" << endl;
			ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << item.offset << "],eax" << endl;
			ss_proc << "\t" << "pop ebp" << endl;

		}
		ss_proc << "\t" << "pop" << " esi" << endl;
		ss_proc << "\t" << "pop" << " edi" << endl;
		ss_proc << "\t" << "pop" << " edx" << endl;
		ss_proc << "\t" << "pop" << " ecx" << endl;
		ss_proc << "\t" << "pop" << " ebx" << endl;
		ss_proc << "\t" << "pop" << " eax" << endl;
		ss_proc << "\t" << "mov" << " esp,ebp" << endl;
		ss_proc << "\t" << "pop" << " ebp" << endl;
		ss_proc << "\t" << "ret" << endl;
	}
	else
	{
		ss_proc << "\t" << "mov" << " esp,ebp" << endl;
		ss_proc << "\t" << "pop" << " ebp" << endl;
		ss_proc << "\t" << "ret" << endl;
	}
	ss_proc << currentProc << " endp" << endl;
	if (ss_proc.str().find("@need@to@add") != string::npos)
	{
		stringstream temp;
		temp << asmStack.top().currentoffset;
		ss_code << ss_proc.str().replace(ss_proc.str().find("@need@to@add"), 12, temp.str()) << endl;
	}
	else
	{
		ss_code << ss_proc.str() << endl;
	}
	asmStack.pop();
	if (!asmStack.empty())
	{
		currentProc = asmStack.top().procName;
	}
	currentLevel--;
	memManage.pop_back();
}
void gen_asm_alloc(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	int paramnum = 1;
	memManageItem item = { iter->answer.name,-1,currentLevel,0 ,iter->answer.isvar };
	int mark = insert_memManage(item);
	if (mark)
	{
		ss_proc << asmStack.top().ss << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 + (asmStack.size() - 1) * 4 << "]" << endl;
	}
	else
	{
		ss_proc << asmStack.top().ss;
	}
	iter++;
	while (iter->op == q_alloc)
	{
		
		memManageItem item = { iter->answer.name,-1,currentLevel,0 ,iter->answer.isvar };
		int mark = insert_memManage(item);
		if (mark)
		{
			ss_proc << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 + (asmStack.size() - 1) * 4 << "]" << endl;
		}
		iter++;
	}
	asmStack.top().ss = ss_proc.str();
	update_memManage({ asmStack.top().procName, -1, currentLevel,0,0,0,4 * (paramnum - 1) });
}
void gen_asm_local(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	
	memManageItem item = { iter->answer.name,-1,currentLevel,0 };
	int mark = insert_memManage(item);
	if (mark)
	{
		if (iter->answer.dimension == 1)
		{
			ss_proc << asmStack.top().ss;
			for (int i = 0; i <= iter->answer.upperbound; i++)
			{
				//ss_proc << "\t" << "push" << " 0" << endl;
			}
			asmStack.top().currentoffset -= 4 * iter->answer.upperbound;
		}
		else
		{
			ss_proc << asmStack.top().ss;// << "\t" << "push" << " 0" << endl;
		}
		
	}
	else
	{
		ss_proc << asmStack.top().ss;
	}
	iter++;
	while (iter->op == q_local)
	{
		
		memManageItem item = { iter->answer.name,-1,currentLevel,0 };
		int mark = insert_memManage(item);
		if (mark)
		{
			if (iter->answer.dimension == 1)
			{
				for (int i = 0; i <= iter->answer.upperbound; i++)
				{
					//ss_proc << "\t" << "push" << " 0" << endl;
				}
				asmStack.top().currentoffset -= 4 * iter->answer.upperbound;
			}
			else
			{
				//ss_proc << "\t" << "push" << " 0" << endl;
			}
		}
		iter++;
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_label(string label)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss << label << ":" << endl;
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_array(vector<quaternion>::iterator &iter)
{
	memManageItem arg1 = find_memManage(iter->arg1.name);
	operand arg2 = iter->arg2;
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	if (arg2.type == _constant)
	{
		int regnum = alloc_register();
		if (regnum >= 0)
		{
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "push ebp" << endl;
				ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
			}
			ss_proc << "\t" << "lea " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset - 4 * arg2.value << "]" << endl;
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "pop ebp" << endl;
			}
			memManageItem ans = { iter->answer.name,regnum,currentLevel,0 ,1 };
			insert_memManage(ans);
		} 
		else
		{
			asmStack.top().currentoffset -= 4;
			int pos = asmStack.top().currentoffset;
			memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
			insert_memManage(ans, 0);
			ss_proc << "\t" << "push eax" << endl;
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "push ebp" << endl;
				ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
			}
			ss_proc << "\t" << "lea " << "eax,[ebp" << setiosflags(ios::showpos) << arg1.offset - 4 * arg2.value << "]" << endl;
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "pop ebp" << endl;
			}
			ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << pos << "],eax" << endl;
			ss_proc << "\t" << "pop eax" << endl;
			
		}
		
	} 
	else
	{
		memManageItem arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)
		{
			if (arg2.reg < 0)
			{
				int regnum = alloc_register();
				if (regnum >= 0)
				{
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
					ss_proc << "\t" << "shl " << registername[regnum] << ", 2" << endl;
					ss_proc << "\t" << "neg " << registername[regnum] << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}

					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "+" << registername[regnum] << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					memManageItem ans = { iter->answer.name,regnum,currentLevel,0 ,1 };
					insert_memManage(ans);
				}
				else
				{
					asmStack.top().currentoffset -= 4;
					int pos = asmStack.top().currentoffset;
					memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
					insert_memManage(ans, 0);
					ss_proc << "\t" << "push eax" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov eax,[eax]" << endl;
					ss_proc << "\t" << "shl eax, 2" << endl;
					ss_proc << "\t" << "neg eax" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << "eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "+eax]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << pos << "],eax" << endl;
					ss_proc << "\t" << "pop eax" << endl;

				}
			}
			else
			{
				int regnum = alloc_register();
				if (regnum >= 0)
				{

					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[arg2.reg] << "]" << endl;
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
					ss_proc << "\t" << "shl " << registername[regnum] << ",2" << endl;
					ss_proc << "\t" << "neg " << registername[regnum] << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "+" << registername[regnum] << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					memManageItem ans = { iter->answer.name,regnum,currentLevel,0 ,1 };
					insert_memManage(ans);
				}
				else
				{
					asmStack.top().currentoffset -= 4;
					int pos = asmStack.top().currentoffset;
					memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
					insert_memManage(ans, 0);
					ss_proc << "\t" << "push eax" << endl;

					ss_proc << "\t" << "mov eax,[" << registername[arg2.reg] << "]" << endl;
					ss_proc << "\t" << "mov eax,[eax]" << endl;
					ss_proc << "\t" << "shl eax,2" << endl;
					ss_proc << "\t" << "neg eax" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << "eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "+eax]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << pos << "],eax" << endl;
					ss_proc << "\t" << "pop eax" << endl;

				}
			}
		}
		else
		{
			if (arg2.reg < 0)
			{
				int regnum = alloc_register();
				if (regnum >= 0)
				{
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "shl " << registername[regnum] << ", 2" << endl;
					ss_proc << "\t" << "neg " << registername[regnum] << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}

					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "+" << registername[regnum] << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					memManageItem ans = { iter->answer.name,regnum,currentLevel,0 ,1 };
					insert_memManage(ans);
				}
				else
				{
					asmStack.top().currentoffset -= 4;
					int pos = asmStack.top().currentoffset;
					memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
					insert_memManage(ans, 0);
					ss_proc << "\t" << "push eax" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "shl eax, 2" << endl;
					ss_proc << "\t" << "neg eax" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << "eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "+eax]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << pos << "],eax" << endl;
					ss_proc << "\t" << "pop eax" << endl;

				}
			}
			else
			{
				int regnum = alloc_register();
				if (regnum >= 0)
				{

					ss_proc << "\t" << "shl " << registername[arg2.reg] << ",2" << endl;
					ss_proc << "\t" << "neg " << registername[arg2.reg] << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << registername[arg2.reg] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "+" << registername[arg2.reg] << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					memManageItem ans = { iter->answer.name,regnum,currentLevel,0 ,1 };
					insert_memManage(ans);
				}
				else
				{
					asmStack.top().currentoffset -= 4;
					int pos = asmStack.top().currentoffset;
					memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
					insert_memManage(ans, 0);
					ss_proc << "\t" << "push eax" << endl;

					ss_proc << "\t" << "mov eax," << registername[arg2.reg] << endl;
					ss_proc << "\t" << "shl eax,2" << endl;
					ss_proc << "\t" << "neg eax" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "lea " << "eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "+eax]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					ss_proc << "\t" << "mov " << "[ebp" << setiosflags(ios::showpos) << pos << "],eax" << endl;
					ss_proc << "\t" << "pop eax" << endl;

				}
			}
		}
	}
	

	asmStack.top().ss = ss_proc.str();
}
void gen_asm_mov(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	string s_arg2;
	ss_proc << asmStack.top().ss;
	memManageItem arg1 = find_memManage(iter->arg1.name);
	int alloc_success = -1;
	//arg2区分
	if (iter->arg2.type == _constant)//reg2是常数
	{		
		stringstream ss;
		ss << iter->arg2.value;
		ss >> s_arg2;
	}
	else
	{
		memManageItem arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)//arg2是数组
		{
			if (arg2.reg < 0)//arg2存在内存中
			{
				int regnum = alloc_register();
				alloc_success = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov " << "ebx" << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov ebx,[ebx]" << endl;
					s_arg2 = "ebx";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
					s_arg2 = registername[regnum];
				}
			}
			else
			{
				s_arg2 = "[";
				s_arg2.append(registername[arg2.reg]);
				s_arg2.append("]");

				free_register(arg1.reg);
				free_memManage(arg1.name);
			}

		}
		else
		{
			if (arg2.reg < 0)//arg2存在内存中
			{
				int regnum = alloc_register();
				alloc_success = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov " << "ebx" << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					s_arg2 = "ebx";
				}
				else
				{
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					if (arg2.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					s_arg2 = registername[regnum];
				}
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}
	}
	//arg1区分
	if (iter->arg1.type == _function)
	{
		ss_proc << "\t" << "push eax" << endl;
		ss_proc << "\t" << "mov eax," << s_arg2 << endl;
		ss_proc << "\t" << "mov [ebp-4],eax" << endl;
		ss_proc << "\t" << "pop eax" << endl;
	} 
	else
	{
		if (arg1.isRef)//arg1是数组
		{
			if (arg1.reg < 0)//arg1存在内存中
			{
				int regnum = alloc_register();
				if (regnum < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "mov " << "eax" << ",[ebp" << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov dword ptr " << "[eax]," << s_arg2 << endl;
					ss_proc << "\t" << "pop eax" << endl;
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov dword ptr [" << registername[regnum] << "]," << s_arg2 << endl;
					free_register(regnum);
				}
			}
			else
			{
				ss_proc << "\t" << "mov dword ptr [" << registername[arg1.reg] << "]," << s_arg2 << endl;
				free_register(arg1.reg);
				free_memManage(arg1.name);
			}

		}
		else
		{
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "push ebp" << endl;
				ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
			}
			ss_proc << "\t" << "mov dword ptr [ebp" << setiosflags(ios::showpos) << arg1.offset << "]," << s_arg2 << endl;
			if (arg1.level < currentLevel)
			{
				ss_proc << "\t" << "pop ebp" << endl;
			}
		}
	}
	
	//arg2恢复现场
	if (iter->arg2.type != _constant)
	{
		memManageItem arg2 = find_memManage(iter->arg2.name);
		if (arg2.reg < 0)//arg2存在内存中
		{
			if (alloc_success != -1)
			{
				free_register(alloc_success);
			}
			else
			{
				ss_proc << "\t" << "pop ebx" << endl;
			}
		}
		
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_add(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer;
	int mark1 = 0, mark2 = 0;
	int regnum = alloc_register();
	if (regnum < 0)
	{
		answer = { iter->answer.name,-1,currentLevel };
		insert_memManage(answer);
	}
	string s_arg1, s_arg2;
	if (iter->arg1.type == _constant)
	{
		stringstream ss;
		ss << iter->arg1.value;
		ss >> s_arg1;
	}
	else
	{
		arg1 = find_memManage(iter->arg1.name);
		if (arg1.isRef)
		{
			if (arg1.reg < 0)
			{

				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				ss_proc << "\t" << "mov eax,[eax]" << endl;
				s_arg1 = "eax";
				mark1 = 1;
			}
			else
			{
				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov eax,[" << registername[arg1.reg] << "]" << endl;
				s_arg1 = "eax";
				mark1 = 1;
			}
		} 
		else
		{
			if (arg1.reg < 0)
			{

					ss_proc << "\t" << "push eax" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					s_arg1 = "eax";
					mark1 = 1;
			}
			else
			{
				s_arg1 = registername[arg1.reg];
			}
		}
		
	}
	if (iter->arg2.type == _constant)
	{
		stringstream ss;
		ss << iter->arg2.value;
		ss >> s_arg2;
	}
	else
	{
		arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)
		{
			if (arg2.reg < 0)
			{

				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				ss_proc << "\t" << "mov ebx,[ebx]" << endl;
				s_arg2 = "ebx";
				mark2 = 1;
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[" << registername[arg2.reg] << "]" << endl;
				s_arg2 = "ebx";
				mark2 = 1;
			}
		}
		else
		{
			if (arg2.reg < 0)
			{

				ss_proc << "\t" << "push ebx" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg2 = "ebx";
				mark2 = 1;
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}

	}
	
	if (regnum < 0)
	{
		answer = find_memManage(answer.name);
		ss_proc << "\t" << "mov dword ptr [ebp" << answer.offset << "]," << s_arg1 << endl;
		ss_proc << "\t" << "add dword ptr [ebp" << answer.offset << "]," << s_arg2 << endl;
	}
	else
	{
		answer = { iter->answer.name,regnum,currentLevel };
		insert_memManage(answer, 0);
		ss_proc << "\t" << "mov " << registername[regnum] << "," << s_arg1 << endl;
		ss_proc << "\t" << "add " << registername[regnum] << "," << s_arg2 << endl;
	}
	
	//恢复现场
	if (mark2 == 1)
	{
		ss_proc << "\t" << "pop ebx" << endl;
	}
	if (mark1 == 1)
	{
		ss_proc << "\t" << "pop eax" << endl;
	}

	asmStack.top().ss = ss_proc.str();
}
void gen_asm_sub(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer;
	int mark1 = 0, mark2 = 0;
	int regnum = alloc_register();
	if (regnum < 0)
	{
		answer = { iter->answer.name,-1,currentLevel };
		insert_memManage(answer);
	}
	string s_arg1, s_arg2;
	if (iter->arg1.type == _constant)
	{
		stringstream ss;
		ss << iter->arg1.value;
		ss >> s_arg1;
	}
	else
	{
		arg1 = find_memManage(iter->arg1.name);
		if (arg1.isRef)
		{
			if (arg1.reg < 0)
			{

				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				ss_proc << "\t" << "mov eax,[eax]" << endl;
				s_arg1 = "eax";
				mark1 = 1;
			}
			else
			{
				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov eax,[" << registername[arg1.reg] << "]" << endl;
				s_arg1 = "eax";
				mark1 = 1;
			}
		}
		else
		{
			if (arg1.reg < 0)
			{

				ss_proc << "\t" << "push eax" << endl;
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg1 = "eax";
				mark1 = 1;
			}
			else
			{
				s_arg1 = registername[arg1.reg];
			}
		}

	}
	if (iter->arg2.type == _constant)
	{
		stringstream ss;
		ss << iter->arg2.value;
		ss >> s_arg2;
	}
	else
	{
		arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)
		{
			if (arg2.reg < 0)
			{

				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				ss_proc << "\t" << "mov ebx,[ebx]" << endl;
				s_arg2 = "ebx";
				mark2 = 1;
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[" << registername[arg2.reg] << "]" << endl;
				s_arg2 = "ebx";
				mark2 = 1;
			}
		}
		else
		{
			if (arg2.reg < 0)
			{

				ss_proc << "\t" << "push ebx" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg2 = "ebx";
				mark2 = 1;
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}

	}

	if (regnum < 0)
	{
		answer = find_memManage(answer.name);
		ss_proc << "\t" << "mov [ebp" << answer.offset << "]," << s_arg1 << endl;
		ss_proc << "\t" << "sub dword ptr [ebp" << answer.offset << "]," << s_arg2 << endl;
	}
	else
	{
		answer = { iter->answer.name,regnum,currentLevel };
		insert_memManage(answer, 0);
		ss_proc << "\t" << "mov " << registername[regnum] << "," << s_arg1 << endl;
		ss_proc << "\t" << "sub " << registername[regnum] << "," << s_arg2 << endl;
	}

	//恢复现场
	if (mark2 == 1)
	{
		ss_proc << "\t" << "pop ebx" << endl;
	}
	if (mark1 == 1)
	{
		ss_proc << "\t" << "pop eax" << endl;
	}

	asmStack.top().ss = ss_proc.str();
}
void gen_asm_mul(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer;
	int mark1 = 0, mark2 = 0, free1 = -1, free2 = -1;
	int regnum = alloc_register();
	if (regnum < 0)
	{
		answer = { iter->answer.name,-1,currentLevel };
		insert_memManage(answer);
	}
	string s_arg1, s_arg2;
	if (iter->arg1.type == _constant)
	{
		stringstream ss;
		ss << iter->arg1.value;
		ss >> s_arg1;
	}
	else
	{
		arg1 = find_memManage(iter->arg1.name);
		if (arg1.isRef)
		{
			if (arg1.reg < 0)
			{
				free1 = alloc_register();
				if (free1 < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov eax,[eax]" << endl;
					s_arg1 = "eax";
					mark1 = 1;
				} 
				else
				{
					ss_proc << "\t" << "mov " << registername[free1] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[free1] << ",[" << registername[free1] << "]" << endl;
					s_arg1 = registername[free1];
				}
				
			}
			else
			{
				s_arg1 = "[";
				s_arg1.append(registername[arg1.reg]);;
				s_arg1.append("]");
			}
		}
		else
		{
			if (arg1.reg < 0)
			{
				stringstream ss;
				ss_proc << "\t" << "push eax" << endl;
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov eax," << "[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg1 = "eax";
				mark1 = 1;
			}
			else
			{
				s_arg1 = registername[arg1.reg];
			}
		}

	}
	if (iter->arg2.type == _constant)
	{
		stringstream ss;
		ss << iter->arg2.value;
		ss >> s_arg2;
	}
	else
	{
		arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)
		{
			if (arg2.reg < 0)
			{
				free2 = alloc_register();
				if (free2 < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov ebx,[ebx]" << endl;
					s_arg2 = "ebx";
					mark2 = 1;
				} 
				else
				{
					ss_proc << "\t" << "mov " << registername[free2] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[free2] << ",[" << registername[free2] << "]" << endl;
					s_arg2 = registername[free2];
				}
				
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[" << registername[arg2.reg] << "]" << endl;
				ss_proc << "\t" << "mov ebx,[ebx]" << endl;
				s_arg2 = "ebx";
				mark2 = 1;
			}
		}
		else
		{
			if (arg2.reg < 0)
			{
				ss_proc << "\t" << "push ebx" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg2 = "ebx";
				mark2 = 1;
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}

	}
	
	if (regnum < 0)
	{
		answer = find_memManage(answer.name);
		ss_proc << "\t" << "push ecx" << endl;
		ss_proc << "\t" << "mov ecx," << s_arg1 << endl;
		ss_proc << "\t" << "imul " << "ecx" << "," << s_arg2 << endl;
		ss_proc << "\t" << "mov [ebp" << setiosflags(ios::showpos) << answer.offset << "],ecx" << endl;
		ss_proc << "\t" << "pop ecx" << endl;
	}
	else
	{
		answer = { iter->answer.name,regnum,currentLevel };
		insert_memManage(answer, 0);
		ss_proc << "\t" << "mov " << registername[regnum] << "," << s_arg1 << endl;
		ss_proc << "\t" << "imul " << registername[regnum] << "," << s_arg2 << endl;
	}
	if (free1 >= 0)
	{
		free_register(free1);
	}
	if (free2 >= 0)
	{
		free_register(free2);
	}
	if (mark2)
	{
		ss_proc << "\t" << "pop ebx" << endl;
	}
	if (mark1)
	{
		ss_proc << "\t" << "pop eax" << endl;
	}


	asmStack.top().ss = ss_proc.str();
}
void gen_asm_div(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer;
	int regnum = alloc_register();
	if (regnum < 0)
	{
		answer = { iter->answer.name,-1,currentLevel };
		insert_memManage(answer);
	}
	ss_proc << "\t" << "push edx" << endl;
	ss_proc << "\t" << "push eax" << endl;
	//被除数
	if (iter->arg1.type == _constant)
	{
		ss_proc << "\t" << "mov eax," << iter->arg1.value << endl;
	}
	else
	{
		arg1 = find_memManage(iter->arg1.name);
		if (arg1.isRef)
		{
			if (arg1.reg < 0)
			{
				ss_proc << "\t" << "mov edx,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				ss_proc << "\t" << "mov eax,[edx]" << endl;
			}
			else
			{
				ss_proc << "\t" << "mov eax,[" << registername[arg1.reg] << "]" << endl;
				ss_proc << "\t" << "mov eax,[eax]" << endl;
			}
		}
		else
		{
			if (arg1.reg < 0)
			{
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
				if (arg1.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
			}
			else
			{
				ss_proc << "\t" << "mov eax," << registername[arg1.reg] << endl;
			}
		}
	}
	
	ss_proc << "\t" << "cdq" << endl;
	//除数
	if (iter->arg2.type == _constant)
	{
		ss_proc << "\t" << "push ebx" << endl;
		ss_proc << "\t" << "mov ebx," << iter->arg2.value << endl;
		ss_proc << "\t" << "idiv ebx" << endl;
		ss_proc << "\t" << "pop ebx" << endl;
	}
	else
	{
		arg2 = find_memManage(iter->arg2.name);
		if (arg2.isRef)
		{
			if (arg2.reg < 0)
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				ss_proc << "\t" << "mov ebx,[ebx]" << endl;
				ss_proc << "\t" << "idiv ebx" << endl;
				ss_proc << "\t" << "pop ebx" << endl;
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx,[" << registername[arg2.reg] << "]" << endl;
				ss_proc << "\t" << "idiv ebx" << endl;
				ss_proc << "\t" << "pop ebx" << endl;
			}
		}
		else
		{
			if (arg2.reg < 0)
			{
				ss_proc << "\t" << "push ebx" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				ss_proc << "\t" << "idiv ebx" << endl;
				ss_proc << "\t" << "pop ebx" << endl;
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				ss_proc << "\t" << "mov ebx," << registername[arg2.reg] << endl;
				ss_proc << "\t" << "idiv ebx" << endl;
				ss_proc << "\t" << "pop ebx" << endl;
			}
		}
	}
	//商
	if (regnum < 0)
	{
		answer = find_memManage(answer.name);
		ss_proc << "\t" << "mov [ebp" << setiosflags(ios::showpos) << answer.offset << "],eax" << endl;
	}
	else
	{
		ss_proc << "\t" << "mov " << registername[regnum] << ",eax" << endl;
		answer = { iter->answer.name,regnum,currentLevel };
		insert_memManage(answer, 0);
	}

	ss_proc << "\t" << "pop eax" << endl;
	ss_proc << "\t" << "pop edx" << endl;
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_j(vector<quaternion>::iterator &iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	ss_proc << "\t" << "jmp " << iter->answer.name << endl;
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_jcommon(vector<quaternion>::iterator &iter,string jtype)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	string s_arg1, s_arg2;
	int free1 = -1, free2 = -1, mark1 = 0, mark2 = 0;
	if (iter->arg1.type == _constant)
	{
		stringstream ss;
		ss << iter->arg1.value;
		ss >> s_arg1;
	}
	else
	{
		memManageItem arg1 = find_memManage(iter->arg1.name);
		if (arg1.reg < 0)//arg1在内存
		{
			if (arg1.isRef)//arg1存的是地址
			{
				int regnum = alloc_register();
				if (regnum < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov eax,[eax]" << endl;
					s_arg1 = "eax";
					mark1 = 1;
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
					s_arg1 = registername[regnum];
					free1 = regnum;
				}
			}
			else
			{
				int regnum = alloc_register();
				if (regnum < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					s_arg1 = "eax";
					mark1 = 1;
				}
				else
				{
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "push ebp" << endl;
						ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg1.level - currentLevel - 1) << "]" << endl;
					}
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg1.offset << "]" << endl;
					if (arg1.level < currentLevel)
					{
						ss_proc << "\t" << "pop ebp" << endl;
					}
					s_arg1 = registername[regnum];
					free1 = regnum;
				}
			}

		}
		else
		{
			if (arg1.isRef)
			{
				stringstream ss;
				ss << "[" << registername[arg1.reg] << "]";
				s_arg1 = ss.str();
			}
			else
			{
				s_arg1 = registername[arg1.reg];
			}

		}
	}
	if (iter->arg2.type == _constant)
	{ 
		stringstream ss;
		ss << iter->arg2.value;
		ss >> s_arg2;
	} 
	else
	{
		memManageItem arg2 = find_memManage(iter->arg2.name);
		if (arg2.reg < 0)
		{
			if (arg2.isRef)//arg2存的是地址
			{
				int regnum = alloc_register();
				if (regnum < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov ebx,[ebx]" << endl;
					s_arg2 = "ebx";
					mark2 = 1;

				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
					s_arg2 = registername[regnum];
					free2 = regnum;
				}
			}
			else
			{
				ss_proc << "\t" << "push ebx" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (arg2.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << arg2.offset << "]" << endl;
				if (arg2.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				s_arg2 = "ebx";
				mark2 = 1;
			}
		}
		else
		{
			if (arg2.isRef)
			{
				stringstream ss;
				ss << "[" << registername[arg2.reg] << "]";
				s_arg2 = ss.str();
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}

		}
	}
	
	ss_proc << "\t" << "cmp " << s_arg1 << "," << s_arg2 << endl;
	//恢复
	if (free1 != -1)
	{
		free_register(free1);
	}
	if (free2 != -1)
	{
		free_register(free2);
	}
	if (mark2)
	{
		ss_proc << "\t" << "pop ebx" << endl;
	}
	if (mark1)
	{
		ss_proc << "\t" << "pop eax" << endl;
	}
	
	ss_proc << "\t" << jtype << " " << iter->answer.name << endl;
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_jne(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "jne");
}
void gen_asm_jge(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "jge");
}
void gen_asm_jg(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "jg");
}
void gen_asm_je(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "je");
}
void gen_asm_jle(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "jle");
}
void gen_asm_jl(vector<quaternion>::iterator &iter)
{
	gen_asm_jcommon(iter, "jl");
}
void gen_asm_accumulate(vector<quaternion>::iterator &iter)
{
	operand var = iter->arg1;
	operand type = iter->arg2;
	stringstream ss_proc;
	string s;
	ss_proc << asmStack.top().ss;
	if (type.value == 1)
	{
		memManageItem mvar = find_memManage(var.name);
		if (mvar.reg < 0)
		{
			if (mvar.level < currentLevel)
			{
				ss_proc << "\t" << "push ebp" << endl;
				ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (mvar.level - currentLevel - 1) << "]" << endl;
			}
			ss_proc << "\t" << "inc dword ptr [ebp" << setiosflags(ios::showpos) << mvar.offset << "]" << endl;
			if (mvar.level < currentLevel)
			{
				ss_proc << "\t" << "pop ebp" << endl;
			}
		}
		else
		{
			ss_proc << "\t" << "inc " << registername[mvar.reg] << endl;
		}
		
	}
	else
	{
		memManageItem mvar = find_memManage(var.name);
		if (mvar.reg < 0)
		{
			if (mvar.level < currentLevel)
			{
				ss_proc << "\t" << "push ebp" << endl;
				ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (mvar.level - currentLevel - 1) << "]" << endl;
			}
			ss_proc << "\t" << "dec dword ptr [ebp" << setiosflags(ios::showpos) << mvar.offset << "]" << endl;
			if (mvar.level < currentLevel)
			{
				ss_proc << "\t" << "pop ebp" << endl;
			}
		}
		else
		{
			ss_proc << "\t" << "dec " << registername[mvar.reg] << endl;
		}
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_push(vector<quaternion>::iterator &iter)
{
	stack<operand> paramStack;
	stringstream ss_proc;
	int mark = 0;
	ss_proc << asmStack.top().ss;
	do 
	{
		operand temp = iter->answer;
		paramStack.push(temp);
		iter++;
	} while (iter->op == q_push);
	if (iter->op == q_call && iter->answer.name.compare("read") == 0)
	{
		iter++;
		stack<operand> paramStack2;
		while (!paramStack.empty())
		{
			paramStack2.push(paramStack.top());
			paramStack.pop();
		}
		while (!paramStack2.empty())
		{
			operand param = paramStack2.top();
			paramStack2.pop();
			memManageItem item = find_memManage(param.name);

			if (item.isRef)
			{
				if (item.reg < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov ebx,[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
					if (param.type == _integer)
					{
						ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input an integer : \"" << endl << "\t" << "pop eax" << endl;
						ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset intFmt" << endl;
						ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
					}
					else if (param.type == _char)
					{
						ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input a char : \"" << endl << "\t" << "pop eax" << endl;
						ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset charFmt" << endl;
						ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
					}
					ss_proc << "\t" << "mov eax,value" << endl;

					ss_proc << "\t" << "mov [ebx],eax" << endl;
					ss_proc << "\t" << "pop ebx" << endl;
					ss_proc << "\t" << "pop eax" << endl;
				} 
				else
				{
					if (item.reg == 0)
					{
						ss_proc << "\t" << "push eax" << endl;
						ss_proc << "\t" << "push ebx" << endl;
						ss_proc << "\t" << "mov ebx,[eax]" << endl;
						if (param.type == _integer)
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input an integer : \"" << endl << "\t" << "pop eax" << endl;
							ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset intFmt" << endl;
							ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
						}
						else if (param.type == _char)
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input a char : \"" << endl << "\t" << "pop eax" << endl;
							ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset charFmt" << endl;
							ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
						}
						ss_proc << "\t" << "mov eax,value" << endl;
						ss_proc << "\t" << "mov [ebx],eax" << endl;
						ss_proc << "\t" << "pop ebx" << endl;
						ss_proc << "\t" << "pop eax" << endl;
					} 
					else
					{
						ss_proc << "\t" << "push eax" << endl;
						if (param.type == _integer)
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input an integer : \"" << endl << "\t" << "pop eax" << endl;
							ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset intFmt" << endl;
							ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
						}
						else if (param.type == _char)
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input a char : \"" << endl << "\t" << "pop eax" << endl;
							ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset charFmt" << endl;
							ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
						}
						ss_proc << "\t" << "mov eax,value" << endl;
						ss_proc << "\t" << "mov [" << registername[item.reg] << "],eax" << endl;
						ss_proc << "\t" << "pop eax" << endl;
					}
					
				}
			} 
			else
			{
				ss_proc << "\t" << "push eax" << endl;
				if (param.type == _integer)
				{
					ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input an integer : \"" << endl << "\t" << "pop eax" << endl;
					ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset intFmt" << endl;
					ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
				}
				else if (param.type == _char)
				{
					ss_proc << "\t" << "push eax" << endl << "\t" << "print \"input a char : \"" << endl << "\t" << "pop eax" << endl;
					ss_proc << "\t" << "push offset value" << endl << "\t" << "push offset charFmt" << endl;
					ss_proc << "\t" << "call crt_scanf" << endl << "\t" << "add esp,+8" << endl;
				}
				ss_proc << "\t" << "mov eax,value" << endl;
				if (item.level < currentLevel)
				{
					ss_proc << "\t" << "push ebp" << endl;
					ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << setiosflags(ios::showpos) << -4 * (item.level - currentLevel - 1) << "]" << endl;
				}
				ss_proc << "\t" << "mov [ebp" << setiosflags(ios::showpos) << item.offset << "],eax" << endl;
				if (item.level < currentLevel)
				{
					ss_proc << "\t" << "pop ebp" << endl;
				}
				ss_proc << "\t" << "pop eax" << endl;
			}
		}

	}
	else if (iter->op == q_call && iter->answer.name.compare("write") == 0)
	{
		iter++;
		if (paramStack.size() ==2)
		{
			operand param2 = paramStack.top();
			paramStack.pop();
			operand param = paramStack.top();
			paramStack.pop();
			ss_data << "\t" << "string_temp" << string_temp_num++ << " db '" << param.name << "',0" << endl;
			ss_proc << "\t" << "push offset string_temp" << string_temp_num - 1 << endl;
			ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+4" << endl;
			paramStack.push(param2);
		}
		if (paramStack.size() == 1)
		{
			operand param = paramStack.top();
			paramStack.pop();
			if (param.type == _string)
			{
				ss_data << "\t" << "string_temp" << string_temp_num++ << " db '" << param.name << "',0" << endl;
				ss_proc << "\t" << "push offset string_temp" << string_temp_num - 1 << endl;
				ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+4" << endl;
			}
			else if (param.type == _constant)
			{
				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov eax," << param.value << endl;
				if (param.constanttype == _integer)
				{
					ss_proc << "\t" << "push eax" << endl << "\t" << "push offset intFmt" << endl;
					ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
				} 
				else
				{
					ss_proc << "\t" << "push eax" << endl << "\t" << "push offset charFmt" << endl;
					ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
				}
				
			}
			else
			{
				string s;
				memManageItem item = find_memManage(param.name);
				if (item.isRef)
				{
					if (item.reg < 0)
					{
						int regnum = alloc_register();
						if (regnum < 0)
						{
							ss_proc << "\t" << "push eax" << endl;
							ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
							ss_proc << "\t" << "mov eax,[eax]" << endl;
							if (param.type == _integer || ((param.type == _function) && (param.value == _integer)))
							{
								ss_proc << "\t" << "push eax" << endl << "\t" << "push offset intFmt" << endl;
								ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
							}
							else
							{
								ss_proc << "\t" << "push eax" << endl << "\t" << "push offset charFmt" << endl;
								ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
							}
							ss_proc << "\t" << "pop eax" << endl;
						} 
						else
						{
							ss_proc << "\t" << "push eax" << endl;
							ss_proc << "\t" << "push " << registername[regnum] << endl;
							ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
							ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
							if (param.type == _integer || ((param.type == _function) && (param.value == _integer)))
							{
								ss_proc << "\t" << "push " << registername[regnum] << endl << "\t" << "push offset intFmt" << endl;
								ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
							}
							else
							{
								ss_proc << "\t" << "push " << registername[regnum] << endl << "\t" << "push offset charFmt" << endl;
								ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
							}
							
							ss_proc << "\t" << "pop " << registername[regnum] << endl;
							ss_proc << "\t" << "pop eax" << endl;
							free_register(regnum);
						}
					} 
					else
					{
						ss_proc << "\t" << "push eax" << endl;
						ss_proc << "\t" << "mov eax,[" << registername[item.reg] << "]" << endl;
						if (param.type == _integer || ((param.type == _function) && (param.value == _integer)))
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "push offset intFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						else
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "push offset charFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						ss_proc << "\t" << "pop eax" << endl;
					}
				}
				else
				{
					if (item.reg < 0)
					{
						ss_proc << "\t" << "push eax" << endl;
						if (item.level < currentLevel)
						{
							ss_proc << "\t" << "push ebp" << endl;
							ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << setiosflags(ios::showpos) << -4 * (item.level - currentLevel - 1) << "]" << endl;
						}
						ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
						if (item.level < currentLevel)
						{
							ss_proc << "\t" << "pop ebp" << endl;
						}
						if (param.type == _integer || ((param.type == _function) && (param.value == _integer)))
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "push offset intFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						else
						{
							ss_proc << "\t" << "push eax" << endl << "\t" << "push offset charFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						ss_proc << "\t" << "pop eax" << endl;

					}
					else
					{
						ss_proc << "\t" << "push eax" << endl;
						if (param.type == _integer || ((param.type == _function) && (param.value == _integer)))
						{
							ss_proc << "\t" << "push " << registername[item.reg] << endl << "\t" << "push offset intFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						else
						{
							ss_proc << "\t" << "push " << registername[item.reg] << endl << "\t" << "push offset charFmt" << endl;
							ss_proc << "\t" << "call crt_printf" << endl << "\t" << "add esp,+8" << endl;
						}
						
						ss_proc << "\t" << "pop eax" << endl;
					}
				}
			}
			
		} 
	}
	else
	{
		while (!paramStack.empty())
		{
			operand param = paramStack.top();
			string s;
			if (param.type == _constant)
			{
				ss_proc << "\t" << "push " << param.value << endl;
			}
			else
			{
				memManageItem item = find_memManage(paramStack.top().name);
				if (param.isvar == 1)//传址
				{
					if (item.isRef)//变量本身是地址
					{
						if (item.reg < 0)
						{
							ss_proc << "\t" << "push [ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
						}
						else
						{
							ss_proc << "\t" << "push " << registername[item.reg] << endl;
						}
					}
					else//变量是值
					{
						if (item.reg < 0)
						{
							ss_proc << "\t" << "push ebp" << endl;
							if (item.level < currentLevel)
							{
								ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (item.level - currentLevel - 1) << "]" << endl;
							}
							ss_proc << "\t" << "lea ebp,[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
							ss_proc << "\t" << "mov [esp+4], ebp" << endl;
							ss_proc << "\t" << "pop ebp" << endl;

						}
						else
						{
							//error
							errorNum++;
						}
					}
				}
				else//传值
				{
					if (item.isRef)//变量本身是地址
					{
						if (item.reg < 0)//存在内存里
						{
							stringstream ss;
							int regnum = alloc_register();
							if (regnum < 0)
							{
								ss_proc << "\t" << "push eax" << endl;
								ss_proc << "\t" << "mov eax,[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
								ss_proc << "\t" << "mov eax,[eax]" << endl;
								ss_proc << "\t" << "mov [esp+4],eax" << endl;
								ss_proc << "\t" << "pop eax" << endl;
							}
							else
							{
								ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
								ss_proc << "\t" << "mov " << registername[regnum] << ",[" << registername[regnum] << "]" << endl;
								ss_proc << "\t" << "push " << registername[regnum] << endl;
								free_register(regnum);
							}

						}
						else
						{
							stringstream ss;
							ss << "[" << registername[item.reg] << "]";
							s = ss.str();
							ss_proc << "\t" << "push " << s << endl;
						}
					}
					else//变量是值
					{
						if (item.reg < 0)
						{
							if (item.level < currentLevel)
							{
								ss_proc << "\t" << "push ebp" << endl;
								ss_proc << "\t" << "mov ebp,[ebp" << setiosflags(ios::showpos) << -4 * (item.level - currentLevel - 1) << "]" << endl;
							}
							ss_proc << "\t" << "push [ebp" << setiosflags(ios::showpos) << item.offset << "]" << endl;
							if (item.level < currentLevel)
							{
								ss_proc << "\t" << "pop ebp" << endl;
							}
						}
						else
						{
							ss_proc << "\t" << "push " << registername[item.reg] << endl;
						}
					}
				}
			}
			paramStack.pop();
		}
	}
	
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_call(vector<quaternion>::iterator &iter)
{
	memManageItem item = find_memManage(iter->answer.name);
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	int num = 0;
	if (item.isFunc)
	{
		if (currentLevel <= item.level)
		{
			for (int i = 1; i < currentLevel; i++)
			{
				ss_proc << "\t" << "push [ebp+" << 4 * (currentLevel - i + 1) << "]" << endl;
				num++;
			}
			ss_proc << "\t" << "push ebp" << endl;
			num++;
		}
		else
		{
			for (int i = 1; i <= item.level; i++)
			{
				ss_proc << "\t" << "push [ebp+" << 4 * (currentLevel - i + 1) << "]" << endl;
				num++;
			}
		}
	} 
	else
	{
		if (currentLevel < item.level)
		{
			for (int i = 1; i < currentLevel; i++)
			{
				ss_proc << "\t" << "push [ebp+" << 4 * (currentLevel - i + 1) << "]" << endl;
				num++;
			}
			ss_proc << "\t" << "push ebp" << endl;
			num++;
		}
		else
		{
			for (int i = 1; i < item.level; i++)
			{
				ss_proc << "\t" << "push [ebp+" << 4 * (currentLevel - i + 1) << "]" << endl;
				num++;
			}
		}
	}
	
	ss_proc << "\t" << "call " << iter->answer.name << endl;
	ss_proc << "\t" << "add esp," << setiosflags(ios::showpos) << 4 * num + item.paramoffset << endl;
	asmStack.top().ss = ss_proc.str();

	
}
/*********************************************/
void gen_asm()
{
	if (errorNum)
	{
		return;
	}
	fout << "include masm32\\include\\masm32rt.inc" << endl;
	ss_data << ".data" << endl;
	ss_data << "\t" << "item dd 0" << endl;
	ss_data << "\t" << "value dd 0" << endl;
	ss_data << "\t" << "intFmt db ' %d',0" << endl;
	ss_data << "\t" << "charFmt db ' %c',0" << endl;
	ss_code << endl << ".code" << endl;
	ss_code << endl << "start:" << endl;
	ss_code << endl << "call main" << endl << "inkey" << endl << "exit" << endl << endl;
	currentProc = "main";
	currentLevel++;
	stringstream ss_main;
	ss_main << currentProc << " proc" << endl;
	//ss_main << "\t" << "cls" << endl;
	ss_main << "\t" << "push ebp" << endl;
	ss_main << "\t" << "mov ebp,esp  " << endl;
	asmStack.push({ currentProc, ss_main.str() ,0 });
	list<memManageItem> list1;
	memManage.push_back(list1);
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		switch (iter->op)
		{
		case q_add:
			gen_asm_add(iter);
			iter++;
			break;
		case q_sub:
			gen_asm_sub(iter);
			iter++;
			break;
		case q_mul:
			gen_asm_mul(iter);
			iter++;
			break;
		case q_div:
			gen_asm_div(iter);
			iter++;
			break;
		case q_j:
			gen_asm_j(iter);
			iter++;
			break;
		case q_jne:
			gen_asm_jne(iter);
			iter++;
			break;
		case q_jge:
			gen_asm_jge(iter);
			iter++;
			break;
		case q_jg:
			gen_asm_jg(iter);
			iter++;
			break;
		case q_je:
			gen_asm_je(iter);
			iter++;
			break;
		case q_jle:
			gen_asm_jle(iter);
			iter++;
			break;
		case q_jl:
			gen_asm_jl(iter);
			iter++;
			break;
		case q_push:
			gen_asm_push(iter);
			break;
		case q_call:
			gen_asm_call(iter);
			iter++;
			break;
		case q_mov:
			gen_asm_mov(iter);
			iter++;
			break;
		case q_procedure:
			gen_asm_procedure(iter);
			iter++;
			break;
		case q_function:
			gen_asm_procedure(iter);
			iter++;
			break;
		case q_label:
			gen_asm_label(iter->answer.name);
			iter++;
			break;
		case q_array:
			gen_asm_array(iter);
			iter++;
			break;
		case q_accumulate:
			gen_asm_accumulate(iter);
			iter++;
			break;
		case q_begin:
			gen_asm_begin();
			iter++;
			break;
		case q_end:
			gen_asm_end();
			iter++;
			break;
		case q_alloc:
			gen_asm_alloc(iter);
			break;
		case q_local:
			gen_asm_local(iter);
			break;
		}
		
	}
	ss_code << "end start" << endl;
	fout << ss_data.str() << ss_code.str();
}