#include "global.h"

typedef struct _memManageItem
{
	string name;
	int reg;
	int level;
	int offset;
	int isRef;
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
/**************运行时存储分配********************/
int alloc_register()
{
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
	for (list<list<memManageItem>>::iterator iter = memManage.begin(); iter != memManage.end(); iter++)
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
int insert_memManage(memManageItem item)
{
	memManageItem temp = find_memManage_local(item.name);
	if (temp.name.empty())
	{
		if (item.offset == 0)
		{
			asmStack.top().currentoffset -= 4;
			item.offset = asmStack.top().currentoffset;
		}
		memManage.back().push_back(item);
		return 1;
	}
	return 0;
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
void gen_asm_procedure(string name)
{
	stringstream ss_proc;
	currentProc = name;
	currentLevel++;
	ss_proc << currentProc << " proc" << endl;
	ss_proc << "\t" << "push ebp" << endl;
	ss_proc << "\t" << "mov ebp,esp" << endl;
	asmStack.push({ currentProc, ss_proc.str() ,0 });
	list<memManageItem> list1;
	memManage.push_back(list1);
}
void gen_asm_begin()
{
	if (currentLevel > 1)
	{
		stringstream ss_proc;
		ss_proc << asmStack.top().ss;
		ss_proc << "\t" << "push" << " eax" << endl;
		ss_proc << "\t" << "push" << " ebx" << endl;
		ss_proc << "\t" << "push" << " ecx" << endl;
		ss_proc << "\t" << "push" << " edx" << endl;
		ss_proc << "\t" << "push" << " edi" << endl;
		ss_proc << "\t" << "push" << " esi" << endl;
		asmStack.top().ss = ss_proc.str();
	}
	
}
void gen_asm_end()
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	if (currentLevel > 1)
	{	
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
	ss_proc << currentProc << " endp" << endl;
	fout << ss_proc.str() << endl;
	asmStack.pop();
	if (!asmStack.empty())
	{
		currentProc = asmStack.top().procName;
	}
	currentLevel--;
	memManage.pop_back();
}
void gen_asm_alloc(vector<quaternion>::iterator iter)
{
	stringstream ss_proc;
	int paramnum = 1;
	memManageItem item = { iter->answer.name,-1,currentLevel,0 };
	int mark = insert_memManage(item);
	if (mark)
	{
		ss_proc << asmStack.top().ss << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 << "]" << endl;
	}
	else
	{
		ss_proc << asmStack.top().ss;
	}
	iter++;
	while (iter->op == q_alloc)
	{
		
		memManageItem item = { iter->answer.name,-1,currentLevel,0 };
		int mark = insert_memManage(item);
		if (mark)
		{
			ss_proc << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 << "]" << endl;
		}
		iter++;
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_local(vector<quaternion>::iterator iter)
{
	stringstream ss_proc;
	
	memManageItem item = { iter->answer.name,-1,currentLevel,0 };
	int mark = insert_memManage(item);
	if (mark)
	{
		if (iter->answer.dimension == 1)
		{
			ss_proc << asmStack.top().ss;
			for (int i = 0; i < iter->answer.upperbound; i++)
			{
				ss_proc << "\t" << "push" << " 0" << endl;
			}
		}
		else
		{
			ss_proc << asmStack.top().ss << "\t" << "push" << " 0" << endl;
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
				for (int i = 0; i < iter->answer.upperbound; i++)
				{
					ss_proc << "\t" << "push" << " 0" << endl;
				}
			}
			else
			{
				ss_proc << "\t" << "push" << " 0" << endl;
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
void gen_asm_array(vector<quaternion>::iterator iter)
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
			ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << arg1.offset - 4 * arg2.value << "]" << endl;
			memManageItem ans = { iter->answer.name,regnum,currentLevel,arg1.offset - 4 * arg2.value ,1 };
			insert_memManage(ans);
		} 
		else
		{
			asmStack.top().currentoffset -= 4;
			ss_proc << "\t" << "push 0" << endl;
			int pos = asmStack.top().currentoffset;
			memManageItem ans = { iter->answer.name,-1,currentLevel, pos,1 };
			insert_memManage(ans);
			asmStack.top().currentoffset -= 4;
			ss_proc << "\t" << "push eax" << endl;
			ss_proc << "\t" << "mov " << "eax,[ebp" << arg1.offset - 4 * arg2.value << "]" << endl;
			ss_proc << "\t" << "mov " << "[ebp" << pos << "],eax" << endl;
			ss_proc << "\t" << "pop eax" << endl;
			asmStack.top().currentoffset += 4;
			
		}
		
	} 
	else
	{
		//need to code
	}
	

	asmStack.top().ss = ss_proc.str();
}
void gen_asm_mov(vector<quaternion>::iterator iter)
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
					ss_proc << "\t" << "mov " << "ebx" << ",[ebp" << arg2.offset << "]" << endl;
					ss_proc << "\t" << "mov ebx,[ebx]" << endl;
					s_arg2 = "ebx";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << arg2.offset << "]" << endl;
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
					ss_proc << "\t" << "mov " << "ebx" << ",[ebp" << arg2.offset << "]" << endl;
					s_arg2 = "ebx";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << arg2.offset << "]" << endl;
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
	if (arg1.isRef)//arg1是数组
	{
		if (arg1.reg < 0)//arg1存在内存中
		{
			int regnum = alloc_register();
			if (regnum < 0)
			{
				ss_proc << "\t" << "push eax" << endl;
				ss_proc << "\t" << "mov " << "eax" << ",[ebp" << arg1.offset << "]" << endl;
				ss_proc << "\t" << "mov " << "[eax]," << s_arg2 << endl;
				ss_proc << "\t" << "pop eax" << endl;
			}
			else
			{
				ss_proc << "\t" << "mov " << registername[regnum] << ",[ebp" << arg1.offset << "]" << endl;
				ss_proc << "\t" << "mov [" << registername[regnum] << "]," << s_arg2 << endl;
				free_register(regnum);
			}
		}
		else
		{
			ss_proc << "\t" << "mov [" << registername[arg1.reg] << "]," << s_arg2 << endl;
			free_register(arg1.reg);
			free_memManage(arg1.name);
		}

	}
	else
	{
		ss_proc << "\t" << "mov [" << arg1.offset << "]," << s_arg2 << endl;
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
void gen_asm_add(vector<quaternion>::iterator iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer = find_memManage(iter->answer.name);
	int alloc_success1 = -1, alloc_success2 = -1;
	if (answer.reg < 0)
	{
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
			if (arg1.reg < 0)
			{
				int regnum = alloc_register();
				alloc_success1 = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "mov eax,[" << arg1.offset << "]" << endl;
					s_arg1 = "eax";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << arg1.offset << "]" << endl;
					s_arg1 = registername[regnum];
				}
			}
			else
			{
				s_arg1 = registername[arg1.reg];
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
			if (arg2.reg < 0)
			{
				int regnum = alloc_register();
				alloc_success2 = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov ebx,[" << arg2.offset << "]" << endl;
					s_arg2 = "eax";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << arg2.offset << "]" << endl;
					s_arg2 = registername[regnum];
				}
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}
		
		ss_proc << "\t" << "mov [ebp" << answer.offset << "]," << s_arg1 << endl;
		ss_proc << "\t" << "add [ebp" << answer.offset << "]," << s_arg2 << endl;
		//恢复现场
		if (iter->arg1.type != _constant)
		{
			if (arg1.reg < 0)
			{
				if (alloc_success1 == -1)
				{
					ss_proc << "\t" << "pop eax" << endl;
				}
				else
				{
					free_register(alloc_success1);
				}
			}
		}
		if (iter->arg2.type != _constant)
		{
			if (arg2.reg < 0)
			{
				if (alloc_success2 == -1)
				{
					ss_proc << "\t" << "pop ebx" << endl;
				}
				else
				{
					free_register(alloc_success2);
				}
			}
		}
		
	} 
	else
	{
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
			if (arg1.reg < 0)
			{
				stringstream ss;
				ss << "[ebp" << arg1.offset << "]";
				s_arg1 = ss.str();
			}
			else
			{
				s_arg1 = registername[arg1.reg];
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
			if (arg2.reg < 0)
			{
				stringstream ss;
				ss << "[ebp" << arg2.offset << "]";
				s_arg2 = ss.str();
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}
		
		ss_proc << "\t" << "mov " << registername[answer.reg] << "," << s_arg1 << endl;
		ss_proc << "\t" << "add " << registername[answer.reg] << "," << s_arg2 << endl;
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_sub(vector<quaternion>::iterator iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;
	memManageItem arg1;
	memManageItem arg2;
	memManageItem answer = find_memManage(iter->answer.name);
	int alloc_success1 = -1, alloc_success2 = -1;
	if (answer.reg < 0)
	{
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
			if (arg1.reg < 0)
			{
				int regnum = alloc_register();
				alloc_success1 = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push eax" << endl;
					ss_proc << "\t" << "mov eax,[" << arg1.offset << "]" << endl;
					s_arg1 = "eax";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << arg1.offset << "]" << endl;
					s_arg1 = registername[regnum];
				}
			}
			else
			{
				s_arg1 = registername[arg1.reg];
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
			if (arg2.reg < 0)
			{
				int regnum = alloc_register();
				alloc_success2 = regnum;
				if (regnum < 0)
				{
					ss_proc << "\t" << "push ebx" << endl;
					ss_proc << "\t" << "mov ebx,[" << arg2.offset << "]" << endl;
					s_arg2 = "eax";
				}
				else
				{
					ss_proc << "\t" << "mov " << registername[regnum] << ",[" << arg2.offset << "]" << endl;
					s_arg2 = registername[regnum];
				}
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}
		
		ss_proc << "\t" << "mov [ebp" << answer.offset << "]," << s_arg1 << endl;
		ss_proc << "\t" << "sub [ebp" << answer.offset << "]," << s_arg2 << endl;
		//恢复现场
		if (iter->arg1.type != _constant)
		{
			if (arg1.reg < 0)
			{
				if (alloc_success1 == -1)
				{
					ss_proc << "\t" << "pop eax" << endl;
				}
				else
				{
					free_register(alloc_success1);
				}
			}
		}
		if (iter->arg2.type != _constant)
		{
			if (arg2.reg < 0)
			{
				if (alloc_success2 == -1)
				{
					ss_proc << "\t" << "pop ebx" << endl;
				}
				else
				{
					free_register(alloc_success2);
				}
			}
		}
		
	}
	else
	{
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
			if (arg1.reg < 0)
			{
				stringstream ss;
				ss << "[ebp" << arg1.offset << "]";
				s_arg1 = ss.str();
			}
			else
			{
				s_arg1 = registername[arg1.reg];
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
			if (arg2.reg < 0)
			{
				stringstream ss;
				ss << "[ebp" << arg2.offset << "]";
				s_arg2 = ss.str();
			}
			else
			{
				s_arg2 = registername[arg2.reg];
			}
		}
		
		ss_proc << "\t" << "mov " << registername[answer.reg] << "," << s_arg1 << endl;
		ss_proc << "\t" << "sub " << registername[answer.reg] << "," << s_arg2 << endl;
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_mul(vector<quaternion>::iterator iter)
{

}
void gen_asm_div(vector<quaternion>::iterator iter)
{

}
void gen_asm_accumulate(vector<quaternion>::iterator iter)
{
	operand var = iter->arg1;
	operand type = iter->arg2;
	stringstream ss_proc;
	string s;
	ss_proc << asmStack.top().ss;
	if (type.type == 1)
	{
		memManageItem mvar = find_memManage(var.name);
		if (mvar.reg < 0)
		{
			ss_proc << "inc [ebp" << mvar.offset << "]" << endl;
		}
		else
		{
			ss_proc << "inc " << registername[mvar.reg] << endl;
		}
		
	}
	else
	{
		memManageItem mvar = find_memManage(var.name);
		if (mvar.reg < 0)
		{
			ss_proc << "dec [ebp" << mvar.offset << "]" << endl;
		}
		else
		{
			ss_proc << "dec " << registername[mvar.reg] << endl;
		}
	}
	asmStack.top().ss = ss_proc.str();
}
/*********************************************/
void gen_asm()
{
	if (errorNum)
	{
		return;
	}
	fout << "include \\masm32\\include\\masm32rt.inc" << endl;
	fout << endl << ".code" << endl;
	fout << endl << "start:" << endl;
	fout << endl << "call main" << endl << "inkey" << endl << "exit" << endl << endl;
	currentProc = "main";
	currentLevel++;
	stringstream ss_main;
	ss_main << currentProc << " proc" << endl;
	ss_main << "\t" << "cls" << endl;
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
			break;
		case q_sub:
			gen_asm_sub(iter);
			break;
		case q_mul:
			gen_asm_mul(iter);
			break;
		case q_div:
			gen_asm_div(iter);
			break;
		case q_j:
			break;
		case q_jne:
			break;
		case q_jge:
			break;
		case q_jg:
			break;
		case q_je:
			break;
		case q_jle:
			break;
		case q_jl:
			break;
		case q_push:
			break;
		case q_call:
			break;
		case q_return:
			break;
		case q_mov:
			gen_asm_mov(iter);
			break;
		case q_procedure:
			gen_asm_procedure(iter->answer.name);
			break;
		case q_function:
			gen_asm_procedure(iter->answer.name);
			break;
		case q_label:
			gen_asm_label(iter->answer.name);
			break;
		case q_array:
			gen_asm_array(iter);
			break;
		case q_accumulate:
			gen_asm_accumulate(iter);
			break;
		case q_begin:
			gen_asm_begin();
			break;
		case q_end:
			gen_asm_end();
			break;
		case q_alloc:
			gen_asm_alloc(iter);
			break;
		case q_local:
			gen_asm_local(iter);
			break;
		}
		iter++;
	}

}