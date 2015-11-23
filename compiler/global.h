#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
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
	_lbracket, _rbracket, _semicolon, _comma, _fullpoint, _equal, _colon, _assign, _less, _lessequal,
	_lessmore, _more, _moreequal, _string, _character, _max_num
};
enum quaternion_op//��Ԫʽ����������
{
	q_add = 1, q_sub, q_mul, q_div, q_je, q_jne, q_jz, q_jg, q_jge, q_jl, q_jle, q_param, q_call, q_mov
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

extern char sBuffer[1000];//Դ�����л�����
extern int sLine;//Դ���뵱ǰ����
extern int sBufLen;//�л���������
extern int sBufPos;//�л�����ָ��
extern char chs[100];//��ǰ���ڷ����ĵ���
extern char ch;//��һ���ַ�
extern ifstream fin;//Դ�ļ�ָ��
extern unordered_map<string, int> reserved;//�����ֱ�
extern ReservedWord reservedTable[_max_num];//��ʼ�������ֱ�
extern Symbol symbol;//��ǰ����
extern int returnChar;//�Ƿ���Ҫ�˻�һ��char
extern int errorNum;//��������

extern list<unordered_map<string, symItem>> symbolTable;//���ű�
//error.cpp
int error(int);
//getSym.cpp
int getSym();
char getChar();
void printSym();
//main.cpp
void reserved_init();
void input_init();
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
void expression();
void term();
void facter();
void callfunction();



