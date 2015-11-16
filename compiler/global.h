#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cwctype>
#include <map>
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

typedef struct _reserved//保留字结构体
{
	int value;
	char name[20];
} ReservedWord;
typedef struct _symbol//符号结构体
{
	int type;
	char identifier[100];
	int value;
} Symbol;


extern char sBuffer[1000];//源代码行缓冲区
extern int sLine;//源代码当前行数
extern int sBufLen;//行缓冲区长度
extern int sBufPos;//行缓冲区指针
extern char chs[100];//当前正在分析的单词
extern char ch;//下一个字符
extern ifstream fin;//源文件指针
extern map<string, int> reserved;//保留字表
extern ReservedWord reservedTable[_max_num];//初始化保留字表
extern Symbol symbol;//当前单词
extern int returnChar;//是否需要退回一个char
typedef pair <string, int> reservedPair;

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
//var.cpp
void varblock();
//procedure.cpp
void procedureblock();
//function.cpp
void functionblock();
//compound.cpp
void compound();

