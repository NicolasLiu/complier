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
unordered_map<string, int> reserved;//�����ֱ�
ReservedWord reservedTable[_max_num] = { {1,"const"}, {2,"var"},{3,"array"},{4,"of"},
								{5,"integer"},{6,"char"},{7,"procedure"},{8,"function"},
								{9,"for"}, {10,"if"}, {11,"else"}, {12,"then"},
								{13,"do"}, {14,"while"}, {15,"downto"}, {16,"to"},
								{17,"begin"}, {18,"end"}, {19,"read"}, {20,"write"},//�����ֵ���Ϊֹ�����������Ǳ�����
								{21,"identifier"}, {22,"constant"}, {23,"+"}, {24,"-"}, 
								{25,"*"}, {26,"/"}, {27,"("}, {28,")"},
								{29,"["}, {30,"]"}, {31,";"}, {32,","},
								{33,"."}, {34,"="}, {35,":"}, {36,":="},
								{37,"<"}, {38,"<="}, {39,"<>"}, {40,">"},
								{41,">="}, {42,"string"},{43,"character"} };//��ʼ�������ֱ�
int returnChar;//�Ƿ���Ҫ�˻�һ��char

int main()
{
	
	input_init();
	reserved_init();

	addSymTableLevel();
	symTableItem sym = { "a1",{"a1",_const,_integer,1,0,0,0} };
	insertSymTable(sym);
	int rtn = findSymTable("a2");
	cout << rtn << endl;
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