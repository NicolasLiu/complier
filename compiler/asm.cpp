#include "global.h"
int registermark[6] = { 0,0,0,0,0,0 };//EAX,EBX,ECX,EDX,ESI,EDI
typedef struct _memManageItem
{
	string name;
	int reg;
	int level;
	int offset;
} memManageItem;
list<list<memManageItem>> memManage;
int alloc_register()
{
	for (int i = 0; i < 6; i++)
	{
		if (registermark == 0)
		{
			return  i;
		}
	}
	return -1;
}
typedef struct _asmstackframe
{
	string procName;
	string ss;
} asmStackFrame;
stack<asmStackFrame> asmStack;
string currentProc;
int currentLevel = 0;
void gen_asm_procedure(string name)
{
	stringstream ss_proc;
	currentProc = name;
	currentLevel++;
	ss_proc << currentProc << " proc" << endl;
	ss_proc << "\t" << "push ebp" << endl;
	ss_proc << "\t" << "mov ebp,esp" << endl;
	asmStack.push({ currentProc, ss_proc.str() });
}
void gen_asm_begin()
{

}
void gen_asm_end()
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss << currentProc << " endp" << endl;
	fout << ss_proc.str() << endl;
	asmStack.pop();
	if (!asmStack.empty())
	{
		currentProc = asmStack.top().procName;
	}
	currentLevel--;
}
void gen_asm_alloc(list<quaternion>::iterator iter)
{
	stringstream ss_proc;
	int paramnum = 1;
	ss_proc << asmStack.top().ss << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 << "]" << endl;
	iter++;
	while (iter->op == q_alloc)
	{
		ss_proc << "\t" << "push" << " [ebp+" << paramnum++ * 4 + 4 << "]" << endl;
		iter++;
	}
	asmStack.top().ss = ss_proc.str();
}
void gen_asm_local(list<quaternion>::iterator iter)
{
	stringstream ss_proc;
	ss_proc << asmStack.top().ss << "\t" << "push" << " 0" << endl;
	iter++;
	while (iter->op == q_local)
	{
		ss_proc << "\t" << "push" << " 0" << endl;
		iter++;
	}
	asmStack.top().ss = ss_proc.str();
}

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
	asmStack.push({ currentProc, ss_main.str() });
	list<quaternion>::iterator iter = quaternionList.begin();
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
			gen_asm_alloc(iter);
			break;
		}
		iter++;
	}

}