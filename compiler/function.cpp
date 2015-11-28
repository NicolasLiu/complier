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
			error(11);//ȱ��;
		}
	}
}
void functionhead()
{
	char functionname[100];
	int params[MaxParams][2];
	char paramName[MaxParams][100];
	int num = 0;
	getSym();
	if (symbol.type == _identifier)
	{
		symItem symitem = findSymTable(symbol.identifier);
		if (!symitem.name.empty())
		{
			error(42);//�ض���ı�ʶ��
		}
		strcpy(functionname, symbol.identifier);
		symTableItem sym = { functionname,{ functionname,0,_function,0,0,0,0,0 } };
		insertSymTable(sym);
		addSymTableLevel();
		gen_icode(q_function, {}, {}, { _function,0,0,functionname });
		getSym();
		if (symbol.type == _lparenthese)
		{

			do
			{
				formalparam(params, paramName, &num);
				getSym();
			} while (symbol.type == _semicolon);
			if (symbol.type == _rparenthese)
			{
				getSym();
			}
			else
			{
				error(12);//ȱ��)
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
						s.params[i][0] = params[i][0];
						s.params[i][1] = params[i][1];
					}
					symTableItem sym = { functionname, s };
					updateSymTable(sym);
					return;
				}
				else
				{
					error(11);//ȱ��;
				}
			}
			else
			{
				error(28);//�Ƿ��ķ���ֵ����
			}
			
		}
		else
		{
			error(13);//ȱ��:
		}
		
	}
	else
	{
		error(27);//�Ƿ��ĺ�����ʶ��
	}
}