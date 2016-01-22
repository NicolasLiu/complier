#include "global.h"
int errorNum;//错误数量
int errorMark = 0;
string errorinfo[100] = { "","非法的字符","非法的字符串","未知的单词","","","","","","","",
						"缺少;","缺少)","缺少:","缺少=","缺少:=","缺少]","缺少then","缺少while","缺少to/downto","缺少do",
						"缺少end","缺少.","缺少begin","非法的过程标识符","非法的参数","非法的参数类型","非法的函数标识符","非法的返回值类型","非法的标识符","非法的无符号整数",
						"非法的关系运算符","非法的类型","数组格式错误","非法的比较运算符","","","","","","",
						"未找到该标识符","重定义的标识符","var类型参数应对应变量","参数不一致","数组类型缺少[]","非法的数组","赋值操作错误","","",""
};

int error(int code)
{
	cout << "*******************************" << endl;
	cout << "第" << sLine << "行";
	if (code > 40)
	{
		cout << " (语义错误)";
	}
	cout << " : " << errorinfo[code] << endl;
	cout << "*******************************" << endl;
	errorNum++;
	if (code <= 40)
	{
		
		while (symbol.type != _semicolon && symbol.type != _end)
		{
			getSym();
		}
		if (symbol.type == _semicolon)
		{
			errorMark = 1;
		} 
		else
		{
			errorMark = 2;
		}
	}
	
	return code;
}