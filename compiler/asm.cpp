#include "global.h"

typedef struct _memManageItem
{
	string name;
	int reg;
	int level;
	int offset;
} memManageItem;
typedef struct _asmstackframe
{
	string procName;
	string ss;
	int currentoffset;
} asmStackFrame;
stack<asmStackFrame> asmStack;
int registermark[6] = { 0,0,0,0,0,0 };//EAX,EBX,ECX,EDX,ESI,EDI
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
			return  i;
		}
	}
	return -1;
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
		item.offset = asmStack.top().currentoffset;
		asmStack.top().currentoffset -= 4;
		memManage.back().push_back(item);
		return 1;
	}
	return 0;
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
	asmStack.push({ currentProc, ss_proc.str() ,-4 });
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
	memManageItem item = { iter->answer.name,0,currentLevel,0 };
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
		
		memManageItem item = { iter->answer.name,0,currentLevel,0 };
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
	
	memManageItem item = { iter->answer.name,0,currentLevel,0 };
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
		
		memManageItem item = { iter->answer.name,0,currentLevel,0 };
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
	stringstream ss_proc;
	ss_proc << asmStack.top().ss;

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
	asmStack.push({ currentProc, ss_main.str() ,-4 });
	list<memManageItem> list1;
	memManage.push_back(list1);
	vector<quaternion>::iterator iter = quaternionList.begin();
	while (iter != quaternionList.end())
	{
		switch (iter->op)
		{
		case q_add:
			break;
		case q_sub:
			break;
		case q_mul:
			break;
		case q_div:
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
			break;
		case q_accumulate:
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