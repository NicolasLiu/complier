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
unordered_map<string, int> reserved;//保留字表
ReservedWord reservedTable[_max_num] = { {1,"const"}, {2,"var"},{3,"array"},{4,"of"},
								{5,"integer"},{6,"char"},{7,"procedure"},{8,"function"},
								{9,"for"}, {10,"if"}, {11,"else"}, {12,"then"},
								{13,"do"}, {14,"while"}, {15,"downto"}, {16,"to"},
								{17,"begin"}, {18,"end"}, {19,"read"}, {20,"write"},//保留字到此为止，接下来不是保留字
								{21,"identifier"}, {22,"constant"}, {23,"+"}, {24,"-"}, 
								{25,"*"}, {26,"/"}, {27,"("}, {28,")"},
								{29,"["}, {30,"]"}, {31,";"}, {32,","},
								{33,"."}, {34,"="}, {35,":"}, {36,":="},
								{37,"<"}, {38,"<="}, {39,"<>"}, {40,">"},
								{41,">="}, {42,"string"},{43,"character"} };//初始化保留字表
int returnChar;//是否需要退回一个char

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