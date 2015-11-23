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
enum symbolType//Symbol类型
{
	_const = 1, _var, _array, _of, _integer, _char, _procedure, _function, _for, _if, _else, _then, _do, _while,
	_downto, _to, _begin, _end, _read, _write,//保留字到此为止
	_identifier, _constant, _plus, _sub, _multi, _div, _lparenthese, _rparenthese,
	_lbracket, _rbracket, _semicolon, _comma, _fullpoint, _equal, _colon, _assign, _less, _lessequal,
	_lessmore, _more, _moreequal, _string, _character, _max_num
};
enum quaternion_op//四元式操作符类型
{
	q_add = 1, q_sub, q_mul, q_div, q_je, q_jne, q_jz, q_jg, q_jge, q_jl, q_jle, q_param, q_call, q_mov
};

typedef struct _reserved//保留字结构体
{
	int value;
	char name[20];
} ReservedWord;
typedef pair <string, int> reservedPair;
typedef struct _symbol//符号结构体
{
	int type;
	char identifier[100];
	int value;
} Symbol;
typedef struct _symbolitem//符号表中的一项
{
	string name;//标识符
	int constvar;//_const,_var
	int type;//_integer, _char,_procedure, _function
	int value;//值
	int dimension;//维数
	int upperbound;//上界
	int isRef;//是否是引用
	int paramnum;//函数或过程参数数量
	char params[10][100];//函数或过程参数名称
}symItem;
typedef pair <string, symItem> symTableItem;

extern char sBuffer[1000];//源代码行缓冲区
extern int sLine;//源代码当前行数
extern int sBufLen;//行缓冲区长度
extern int sBufPos;//行缓冲区指针
extern char chs[100];//当前正在分析的单词
extern char ch;//下一个字符
extern ifstream fin;//源文件指针
extern unordered_map<string, int> reserved;//保留字表
extern ReservedWord reservedTable[_max_num];//初始化保留字表
extern Symbol symbol;//当前单词
extern int returnChar;//是否需要退回一个char
extern int errorNum;//错误数量

extern list<unordered_map<string, symItem>> symbolTable;//符号表
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



