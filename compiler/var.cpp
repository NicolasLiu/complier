#include "global.h"
void varblock()
{
	getSym();
	do 
	{
		vardefine();
		getSym();
	} while (symbol.type == _identifier);
	if (symbol.type == _semicolon)
	{
		return;
	}
	else
	{
		error(1);//Ӧ�ԷֺŽ�β
	}
}
void vardefine()
{
	char names[100][100];
	int i = 0;
	while (symbol.type == _identifier)
	{
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
					}
					else
					{
						for (; i > 0; i--)
						{
							symTableItem sym = { names[i - 1],{ names[i - 1],_var,symbol.type } };
							insertSymTable(sym);
						}
						
					}
					
				}
				else
				{
					error(4);//�Ƿ�������
				}
			}
		}
	}
	error(1);//�Ƿ��ı�ʶ��
}