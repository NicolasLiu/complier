#include "global.h"
#define sourceName "input.txt"

char sBuffer[1000];//源代码行缓冲区
int sLine;//源代码当前行数
int sBufLen;//行缓冲区长度
int sBufPos;//行缓冲区指针
char chs[100];//当前正在分析的单词
char ch;//下一个字符
Symbol symbol;//当前读取到的单词
ifstream fin;//源文件指针
map<string, int> reserved;//保留字表

int main()
{
	
	input_init();
	reserved_init();

	
	getSym();
	return 0;
}
void input_init()
{
	fin.open(sourceName);
	getChar();
}
void reserved_init()
{
	reserved.insert(reservedPair("integer", _integer));
	reserved.insert(reservedPair("char", _char));
	reserved.insert(reservedPair("const", _const));
	reserved.insert(reservedPair("var", _var));
	reserved.insert(reservedPair("procedure", _procedure));
	reserved.insert(reservedPair("function", _function));
	reserved.insert(reservedPair("for", _for));
	reserved.insert(reservedPair("if", _if));
	reserved.insert(reservedPair("then", _then));
	reserved.insert(reservedPair("else", _else));
	reserved.insert(reservedPair("while", _while));
	reserved.insert(reservedPair("do", _do));
	reserved.insert(reservedPair("downto", _downto));
	reserved.insert(reservedPair("to", _to));
	reserved.insert(reservedPair("begin", _begin));
	reserved.insert(reservedPair("end", _end));
	reserved.insert(reservedPair("read", _read));
	reserved.insert(reservedPair("write", _write));
}