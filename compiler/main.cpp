#include "global.h"
#define sourceName "input.txt"
#define asmName "out.asm"

char sBuffer[1000];//Դ�����л�����
int sLine;//Դ���뵱ǰ����
int sBufLen;//�л���������
int sBufPos;//�л�����ָ��
char chs[100];//��ǰ���ڷ����ĵ���
char ch;//��һ���ַ�
Symbol symbol;//��ǰ��ȡ���ĵ���
ifstream fin;//Դ�ļ�ָ��
ofstream fout;//����ļ�ָ��

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

