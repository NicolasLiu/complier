#include "global.h"
#define sourceName "input.txt"

char sBuffer[1000];//Դ�����л�����
int sLine;//Դ���뵱ǰ����
int sBufLen;//�л���������
int sBufPos;//�л�����ָ��
char chs[100];//��ǰ���ڷ����ĵ���
char ch;//��һ���ַ�
Symbol symbol;//��ǰ��ȡ���ĵ���
ifstream fin;//Դ�ļ�ָ��



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