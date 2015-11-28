#include "global.h"
void varblock()
{
	getSym();
	do 
	{
		vardefine();
		getSym();
	} while (symbol.type == _identifier);
}
void vardefine()
{
	char names[100][100];
	int i = 0;
	while (symbol.type == _identifier)
	{
		symItem sym = findSymTable(symbol.identifier);
		if (!sym.name.empty())
		{
			error(42);//重定义的标识符
		}
		strcpy(names[i++], symbol.identifier);
		getSym();
		if (symbol.type == _comma)
		{
			getSym();
		}
		else
		{
			if (symbol.type == _colon)
			{
				getSym();
				if (symbol.type == _integer || symbol.type == _char || symbol.type == _array)
				{
					if (symbol.type == _array)
					{
						int upperbound;
						int type;
						getSym();
						if (symbol.type == _lbracket)
						{
							getSym();
							if (symbol.type == _constant)
							{
								upperbound = symbol.value;
								getSym();
								if (symbol.type == _rbracket)
								{
									getSym();
									if (symbol.type == _of)
									{
										getSym();
										if (symbol.type == _char || symbol.type == _integer)
										{
											type = symbol.type;
										}
										else
										{
											error(32);//非法的类型
										}
									}
									else
									{
										error(1);//数组格式错误
									}
								}
								else
								{
									error(1);//数组格式错误
								}
							}
							else
							{
								error(1);//数组格式错误
							}
						}
						else
						{
							error(1);//数组格式错误
						}
						for (; i > 0; i--)
						{
							symTableItem sym = { names[i - 1],{ names[i - 1],_var,type ,0,1,upperbound} };
							insertSymTable(sym);
						}
						getSym();
						if (symbol.type != _semicolon)
						{
							error(11);//缺少;
						}
						return;
					}
					else
					{
						for (; i > 0; i--)
						{
							symTableItem sym = { names[i - 1],{ names[i - 1],_var,symbol.type } };
							insertSymTable(sym);
						}
						getSym();
						if (symbol.type != _semicolon)
						{
							error(11);//缺少;
						}
						return;
					}
					
				}
				else
				{
					error(32);//非法的类型
				}
			}
		}
	}
	error(29);//非法的标识符
}