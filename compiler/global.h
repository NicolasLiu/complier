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


extern char sBuffer[1000];//Դ�����л�����
extern int sLine;//Դ���뵱ǰ����
extern int sBufLen;//�л���������
extern int sBufPos;//�л�����ָ��
extern char chs[100];//��ǰ���ڷ����ĵ���
extern char ch;//��һ���ַ�
extern ifstream fin;//Դ�ļ�ָ��
extern map<string, int> reserved;//�����ֱ�
extern Symbol symbol;//��ǰ����
typedef pair <string, int> reservedPair;


int getSym();
char getChar();
void reserved_init();
void input_init();

