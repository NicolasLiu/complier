#include "global.h"
int getSym()
{
	if (!returnChar) 
	{
		getChar();
	}
	else
	{
		returnChar = 0;
	}
	while (isspace(ch) || ch == 0)
	{
		getChar();
	}
	if (isalpha(ch))
	{
		int i = 0;
		chs[i++] = ch;
		while (isalnum(getChar()))
		{
			chs[i++] = ch;
		}
		chs[i] = 0;
		returnChar = 1;
		int type = reserved[chs];
		if (type != 0)
		{
			symbol.type = type;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			symbol.type = _identifier;
			symbol.value = 0;
			strcpy(symbol.identifier, chs);
			symbol.identifier[strlen(chs)] = 0;
		}

	}
	else if (isdigit(ch))
	{
		int n = ch - '0';
		while (isdigit(getChar()))
		{
			n = n * 10 + ch - '0';
		}
		returnChar = 1;
		symbol.type = _constant;
		symbol.value = n;
		symbol.identifier[0] = 0;
	}
	else if (ch == ':')
	{
		getChar();
		if (ch == '=')
		{
			symbol.type = _assign;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			returnChar = 1;
			symbol.type = _colon;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
	}
	else if (ch == '>')
	{
		getChar();
		if (ch == '=')
		{
			symbol.type = _moreequal;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			returnChar = 1;
			symbol.type = _more;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
	}
	else if (ch == '<')
	{
		getChar();
		if (ch == '=')
		{
			symbol.type = _lessequal;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else if (ch == '>')
		{
			symbol.type = _lessmore;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			returnChar = 1;
			symbol.type = _less;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
	}
	else if (ch == '"')
	{
		getChar();
		if (ch == '"')
		{
			symbol.type = _string;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			int i = 0;
			symbol.type = _string;
			symbol.value = 0;
			symbol.identifier[i++] = ch;	
			do 
			{
				getChar();
				if (ch < 32 || ch>126)
				{
					error(3);//字符串中包括非法内容
				}
				symbol.identifier[i++] = ch;
			} while (ch != '"');
			symbol.identifier[i - 1] = 0;
		}
		
	}
	else if (ch == '\'')
	{	
		getChar();
		if (isalnum(ch))
		{
			symbol.value = ch;
		}
		else
		{
			error(1);//不是一个字符
		}
		getChar();
		if (ch == '\'')
		{
			symbol.type = _character;
			symbol.identifier[0] = 0;
		}
		else
		{
			error(1);//不是一个字符
			symbol.type = 0;
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
	}
	else if (ch == '+')
	{
		symbol.type = _plus;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '-')
	{
		symbol.type = _sub;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '*')
	{
		symbol.type = _multi;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '/')
	{
		symbol.type = _div;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '(')
	{
		symbol.type = _lparenthese;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == ')')
	{
		symbol.type = _rparenthese;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '[')
	{
		symbol.type = _lbracket;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == ']')
	{
		symbol.type = _rbracket;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == ';')
	{
		symbol.type = _semicolon;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == ',')
	{
		symbol.type = _comma;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '.')
	{
		symbol.type = _fullpoint;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else if (ch == '=')
	{
		symbol.type = _equal;
		symbol.value = 0;
		symbol.identifier[0] = 0;
	}
	else
	{
		error(2);//不是已知的Symbol
	}
	return 0;
}
char getChar()
{
	if (sBufLen == 0 || sBufLen == sBufPos)
	{
		string s;
		getline(fin, s);
		sLine++;
		//s.append(" ");
		sBufLen = s.length();
		strcpy(sBuffer, s.c_str());
		sBufPos = 0;
		return ch = 0;
	}
	ch = sBuffer[sBufPos++];
	return ch;
}

void printSym()
{
	if (symbol.type == _identifier)
	{
		cout << "标识符 " << symbol.identifier << endl;
	}
	else if (symbol.type <= reservedNum)
	{
		cout << "保留字 " << reservedTable[symbol.type - 1].name << endl;
	}
	else if (symbol.type == _constant)
	{
		cout << "无符号整数 " << symbol.value << endl;
	}
	else if (symbol.type == _character)
	{
		cout << "字符 " << symbol.value << endl;
	}
	else if (symbol.type == _string)
	{
		cout << "字符串 " << symbol.identifier << endl;
	}
	else
	{
		cout << "分隔符 " << reservedTable[symbol.type - 1].name << endl;
	}
}