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



int main()
{
	
	input_init();
	reserved_init();

	program();
	print_icode();
	return 0;
}
void input_init()
{
	fin.open(sourceName);
}
void reserved_init()
{
	for (int i = 0; i < reservedNum; i++)
	{
		reserved.insert(reservedPair(reservedTable[i].name, reservedTable[i].value));
	}
}