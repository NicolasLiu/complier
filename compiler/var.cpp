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
			error(42);//�ض���ı�ʶ��
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
											error(32);//�Ƿ�������
										}
									}
									else
									{
										error(1);//�����ʽ����
									}
								}
								else
								{
									error(1);//�����ʽ����
								}
							}
							else
							{
								error(1);//�����ʽ����
							}
						}
						else
						{
							error(1);//�����ʽ����
						}
						for (; i > 0; i--)
						{
							symTableItem sym = { names[i - 1],{ names[i - 1],_var,type ,0,1,upperbound} };
							insertSymTable(sym);
						}
						getSym();
						if (symbol.type != _semicolon)
						{
							error(11);//ȱ��;
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
							error(11);//ȱ��;
						}
						return;
					}
					
				}
				else
				{
					error(32);//�Ƿ�������
				}
			}
		}
	}
	error(29);//�Ƿ��ı�ʶ��
}