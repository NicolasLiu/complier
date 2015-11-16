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
enum symbolType//Symbol����
{
	_const = 1, _var, _array, _of, _integer, _char, _procedure, _function, _for, _if, _else, _then, _do, _while,
	_downto, _to, _begin, _end, _read, _write,//�����ֵ���Ϊֹ
	_identifier, _constant, _plus, _sub, _multi, _div, _lparenthese, _rparenthese,
	_lbracket, _rbracket, _semicolon, _comma, _fullpoint, _equal, _colon, _assign, _less, _lessequal,
	_lessmore, _more, _moreequal, _string, _character, _max_num
};

typedef struct _reserved//�����ֽṹ��
{
	int value;
	char name[20];
} ReservedWord;
typedef struct _symbol//���Žṹ��
{
	int type;
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
extern ReservedWord reservedTable[_max_num];//��ʼ�������ֱ�
extern Symbol symbol;//��ǰ����
extern int returnChar;//�Ƿ���Ҫ�˻�һ��char
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

