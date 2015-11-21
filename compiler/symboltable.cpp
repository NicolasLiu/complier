#include "global.h"

list<unordered_map<string, symItem>> symbolTable;
void addSymTableLevel()
{
	unordered_map<string, symItem> map;
	symbolTable.push_back(map);
}
void insertSymTable(symTableItem sym)
{
	symbolTable.back().insert(sym);
}
int findSymTable(string name)
{
	for(unordered_map<string, symItem> s : symbolTable)
	{
		if (s.end() != s.find(name))
		{
			return 1;
		}
	} 
	return 0;
}
int findSymTableLocal(string name)
{
	unordered_map<string, symItem> s = symbolTable.back();
	if (s.end() != s.find(name))
	{
		return 1;
	}
	return 0;
}