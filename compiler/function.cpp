#include "global.h"
void functionblock()
{
	while (1)
	{
		functionhead();
		childprogram();
		if (symbol.type == _semicolon)
		{
			getSym();
			if (symbol.type != _function)
			{
				break;
			}
		}
		else
		{
			error(1);//Ӧ�ԷֺŽ���
		}
	}
}
void functionhead()
{
	char functionname[100];
	char params[10][100];
	int num = 0;
	getSym();
	if (symbol.type == _identifier)
	{
		strcpy(functionname, symbol.identifier);
		symTableItem sym = { functionname,{ functionname,0,_function,0,0,0,0,0 } };
		insertSymTable(sym);
		addSymTableLevel();
		gen_icode(q_function, {}, {}, { 0,0,functionname });
		getSym();
		if (symbol.type == _lparenthese)
		{

			do
			{
				formalparam(params, &num);
				getSym();
			} while (symbol.type == _semicolon);
			if (symbol.type == _rparenthese)
			{
				getSym();
			}
			else
			{
				error(1);//����˵������ȱ��������
			}

		}
		if (symbol.type == _colon)
		{
			getSym();
			if (symbol.type == _integer || symbol.type == _char)
			{
				int rtnType = symbol.type;
				getSym();
				if (symbol.type == _semicolon)
				{
					symItem s = { functionname,0,_function,rtnType,0,0,0,num };
					for (int i = 0; i < num; i++)
					{
						strcpy(s.params[i], params[i]);
					}
					symTableItem sym = { functionname, s };
					updateSymTable(sym);
					return;
				}
				else
				{
					error(1);//Ӧ�ԷֺŽ���
				}
			}
			else
			{
				error(1);//��������ֵ���ʹ���
			}
			
		}
		else
		{
			error(1);//�����ײ�ȱ��ð��
		}
		
	}
	else
	{
		error(1);//������ʶ������
	}
}