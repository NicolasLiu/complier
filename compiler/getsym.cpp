#include "global.h"
int getSym()
{
	while (isspace(ch))
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
		int type = reserved[chs];
		if (type != 0)
		{
			symbol.type = reservedWord(type);
			symbol.value = 0;
			symbol.identifier[0] = 0;
		}
		else
		{
			symbol.type = _identifier;
			symbol.value = 0;
			strcpy(symbol.identifier, chs);
		}

	}
	else if (isdigit(ch))
	{
		int n = ch - '0';
		while (isdigit(getChar()))
		{
			n = n * 10 + ch - '0';
		}
		symbol.type = _number;
		symbol.value = n;
		symbol.identifier[0] = 0;
	}
	else if (ch == ':')
	{

	}
	else if (ch == '>')
	{

	}
	else if (ch == '<')
	{

	}
	else
	{

	}

	return 0;
}
char getChar()
{
	if (sBufLen == sBufPos)
	{
		fin.getline(sBuffer, 1000);
		sBufLen = (int)fin.gcount();
		sBufPos = 0;
	}
	ch = sBuffer[sBufPos++];
	return ch;
}