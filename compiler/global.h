#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cwctype>
#include <unordered_map>
#include <list>
#include <stdlib.h>

using namespace std;

#define reservedNum 20
enum symbolType//Symbol����
{
	_const = 1, _var, _array, _of, _integer, _char, _procedure, _function, _for, _if, _else, _then, _do, _while,
	_downto, _to, _begin, _end, _read, _write,//�����ֵ���Ϊֹ
	_identifier, _constant, _plus, _sub, _multi, _div, _lparenthese, _rparenthese,
	_lbracket, _rbracket, _semicolon, _comma, _fullpoint, _colon, _assign, _equal, _less, _lessequal,
	_lessmore, _more, _moreequal, _string, _character, _max_num
};
enum quaternion_op//��Ԫʽ����������
{
	q_add = 1, q_sub, q_mul, q_div, q_j, q_jne, q_jge, q_jg, q_je, q_jle, q_jl, q_push, q_call, q_return, q_mov, q_procedure, q_function, q_label, q_array, q_accumulate
};

typedef struct _reserved//�����ֽṹ��
{
	int value;
	char name[20];
} ReservedWord;
typedef pair <string, int> reservedPair;
typedef struct _symbol//���Žṹ��
{
	int type;
	char identifier[100];
	int value;
} Symbol;
typedef struct _symbolitem//���ű��е�һ��
{
	string name;//��ʶ��
	int constvar;//_const,_var
	int type;//_integer, _char,_procedure, _function
	int value;//ֵ
	int dimension;//ά��
	int upperbound;//�Ͻ�
	int isRef;//�Ƿ�������
	int paramnum;//��������̲�������
	char params[10][100];//��������̲�������
}symItem;
typedef pair <string, symItem> symTableItem;
typedef struct _operand//����������
{
	int type;//1��ʾ����,0��ʾ������,2��ʾ�ַ���
	int value;//type==1ʱʹ��
	string name;//type==0,2ʱʹ��

} operand;
typedef struct _quaternion//��Ԫʽ�ṹ
{
	int op;
	operand arg1;
	operand arg2;
	operand answer;
}quaternion;

extern char sBuffer[1000];//Դ�����л�����
extern int sLine;//Դ���뵱ǰ����
extern int sBufLen;//�л���������
extern int sBufPos;//�л�����ָ��
extern char chs[100];//��ǰ���ڷ����ĵ���
extern char ch;//��һ���ַ�
extern ifstream fin;//Դ�ļ�ָ��
extern ofstream fout;//����ļ�ָ��
extern unordered_map<string, int> reserved;//�����ֱ�
extern ReservedWord reservedTable[_max_num];//��ʼ�������ֱ�
extern Symbol symbol;//��ǰ����
extern int returnChar;//�Ƿ���Ҫ�˻�һ��char
extern int errorNum;//��������
extern int temp_var_t;//�м������ʱ�������
extern int temp_label;//�м����label���
extern list<unordered_map<string, symItem>> symbolTable;//���ű�
extern list<quaternion> quaternionList;//��Ԫʽ�б�

//error.cpp
int error(int);
//getSym.cpp
int getSym();
char getChar();
void printSym();
//main.cpp

//program.cpp
void program();
void childprogram();
//const.cpp
void constblock();
void constdefine();
//var.cpp
void varblock();
void vardefine();
//procedure.cpp
void procedureblock();
void procedurehead();
void formalparam(char[][100], int*);
//function.cpp
void functionblock();
void functionhead();
//sentence.cpp
void compound();
void read();
void write();
void forloop();
void dowhile();
void sentence();
void assignment();
void ifsentence();
void callprocedure();
//symboltable.cpp
void addSymTableLevel();
void insertSymTable(symTableItem sym);
void updateSymTable(symTableItem sym);
symItem findSymTable(string name);
symItem findSymTableLocal(string name);
void popSymTableLevel();
//expression.cpp
operand expression();
operand term();
operand facter();
void callfunction();
//i_code.cpp
operand alloc_temp();
operand alloc_label();
void gen_icode(int op, operand arg1, operand arg2, operand answer);
void print_icode();
//asm.cpp
void gen_asm();



