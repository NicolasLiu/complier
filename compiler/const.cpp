#include "global.h"
void constblock()
{

}
void constdefine()
{
	char name[100];
	int value;
	getSym();
	if (symbol.type == _identifier)//��ʶ��
	{
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
			}
			else
			{
				error(4);//�����޷�������
			}
		}
		else
		{
			error(4);//���ǵȺ�
		}
	}
	else
	{
		error(4);//���Ǳ�ʶ��
	}

}