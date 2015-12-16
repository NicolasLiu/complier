#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cwctype>
#include <unordered_map>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <stdlib.h>

using namespace std;

#define reservedNum 20
#define MaxParams 10
enum symbolType//Symbol类型
{
	_const = 1, _var, _array, _of, _integer, _char, _procedure, _function, _for, _if, _else, _then, _do, _while,
	_downto, _to, _begin, _end, _read, _write,//保留字到此为止
	_identifier, _constant, _plus, _sub, _multi, _div, _lparenthese, _rparenthese,
	_lbracket, _rbracket, _semicolon, _comma, _fullpoint, _colon, _assign, _equal, _less, _lessequal,
	_more, _moreequal, _lessmore, _string, _character, _max_num
};
enum quaternion_op//四元式操作符类型
{
	q_add = 1, q_sub, q_mul, q_div, q_j, q_jne, q_jge, q_jg, q_jle, q_jl, q_je, q_push, q_call, q_mov, q_procedure, q_function, q_label, q_array, q_accumulate, q_begin, q_end, q_alloc, q_local
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
	int params[MaxParams][2];//函数或过程参数，[0]表示isRef，[1]表示_integer, _char
}symItem;
typedef pair <string, symItem> symTableItem;
typedef struct _operand//操作数或结果
{
	int type;//_constant,_integer,_char,_string,_function,_procedure
	int value;//_constant,_function时使用
	int isvar;//_integer,_char时使用
	string name;//_integer,_char,_string,_function,_procedure时使用
	int dimension;//维数
	int upperbound;//上界
	int constanttype;//const char,const integer
} operand;
typedef struct _quaternion//四元式结构
{
	int op;
	operand arg1;
	operand arg2;
	operand answer;
}quaternion;

extern char sBuffer[1000];//源代码行缓冲区
extern int sLine;//源代码当前行数
extern int sBufLen;//行缓冲区长度
extern int sBufPos;//行缓冲区指针
extern char chs[100];//当前正在分析的单词
extern char ch;//下一个字符
extern ifstream fin;//源文件指针
extern ofstream fout;//汇编文件指针
extern unordered_map<string, int> reserved;//保留字表
extern ReservedWord reservedTable[_max_num];//初始化保留字表
extern Symbol symbol;//当前单词
extern int returnChar;//是否需要退回一个char
extern int errorNum;//错误数量
extern int temp_var_t;//中间代码临时变量标号
extern int temp_label;//中间代码label标号
extern list<unordered_map<string, symItem>> symbolTable;//符号表
extern vector<quaternion> quaternionList;//四元式列表

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
void formalparam(int[][2], char[][100], int*);
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
operand alloc_temp(int type);
operand alloc_label();
void gen_icode(int op, operand arg1, operand arg2, operand answer);
void print_icode();
//asm.cpp
void gen_asm();
//optimization.cpp
void optimization();



