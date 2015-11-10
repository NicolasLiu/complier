#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cwctype>
#include <map>
#include <stdlib.h>

using namespace std;

enum reservedWord
{
	_integer = 1, _char, _const, _var, _procedure, _function, _for, _if, _then,
	_else, _while, _do, _downto, _to, _begin, _end, _read, _write, _max_r, _identifier,
	_number, _max_num
};
typedef struct _symbol
{
	reservedWord type;
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
extern Symbol symbol;//当前单词
typedef pair <string, int> reservedPair;


int getSym();
char getChar();
void reserved_init();
void input_init();

