#include "global.h"
#define sourceName "input.txt"
#define asmName "out.asm"

char sBuffer[1000];//源代码行缓冲区
int sLine;//源代码当前行数
int sBufLen;//行缓冲区长度
int sBufPos;//行缓冲区指针
char chs[100];//当前正在分析的单词
char ch;//下一个字符
Symbol symbol;//当前读取到的单词
ifstream fin;//源文件指针
ofstream fout;//汇编文件指针

void io_init()
{
	fin.open(sourceName);
	fout.open(asmName);
}
void io_close()
{
	fin.close();
	fout.flush();
	fout.close();
}
void reserved_init()
{
	for (int i = 0; i < reservedNum; i++)
	{
		reserved.insert(reservedPair(reservedTable[i].name, reservedTable[i].value));
	}
}

int main()
{
	
	io_init();
	reserved_init();
	program();
	optimization();
	print_icode();
	gen_asm();
	io_close();
	system("makeit.bat");

	return 0;
}

