#include "global.h"
void constblock()
{
	do 
	{
		constdefine();
	} while (symbol.type == _comma);
	if (errorMark)
	{
		errorMark = 0;
	}
	if (symbol.type == _semicolon)
	{
		return;
	}
	else
	{
		error(11);//ȱ��;
		return;
	}
	
}
void constdefine()
{
	char name[100];
	int value;
	getSym();
	if (symbol.type == _identifier)//��ʶ��
	{
		symItem sym = findSymTableLocal(symbol.identifier);
		if (!sym.name.empty())
		{
			error(42);//�ض���ı�ʶ��
		}
		int k = 1;//����Ĭ��Ϊ��
		strcpy(name, symbol.identifier);
		getSym();
		if (symbol.type == _equal)//=
		{
			getSym();
			if (symbol.type == _plus)//+
			{
				getSym();
				k = 1;
			}
			else if (symbol.type == _sub)//-
			{
				getSym();
				k = -1;
			}

			if (symbol.type == _constant)//�޷�������
			{
				value = k*symbol.value;
				symTableItem sym = { name,{name,_const,_integer,value} };
				insertSymTable(sym);
			}
			else if (symbol.type == _character)//�ַ�
			{
				value = k*symbol.value;
				symTableItem sym = { name,{ name,_const,_char,value } };
				insertSymTable(sym);
			}
			else
			{
				error(30);//�Ƿ����޷�������
				return;
			}
		}
		else
		{
			error(14);//ȱ��=
			return;
		}
	}
	else
	{
		error(29);//�Ƿ��ı�ʶ��
		return;
	}
	getSym();
}