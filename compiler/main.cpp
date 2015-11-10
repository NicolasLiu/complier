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
map<string, int> reserved;//�����ֱ�

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