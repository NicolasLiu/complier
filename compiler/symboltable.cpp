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
void updateSymTable(symTableItem sym)
{
	list<unordered_map<string, symItem>>::reverse_iterator iter;
	for (iter = symbolTable.rbegin(); iter != symbolTable.rend(); iter++)
	{
		unordered_map<string, symItem>::iterator iter2 = iter->find(sym.first);
		if (iter->end() != iter2)
		{
			iter->erase(sym.first);
			iter->insert(sym);
			return;
		}
	}
	symbolTable.back().insert(sym);
}
symItem findSymTable(string name)
{
	list<unordered_map<string, symItem>>::reverse_iterator iter;
	for (iter = symbolTable.rbegin(); iter != symbolTable.rend(); iter++)
	{
		unordered_map<string, symItem>::iterator iter2 = iter->find(name);
		if (iter->end() != iter2)
		{
			return iter2->second;
		}
	} 
	return {};
}
symItem findSymTableLocal(string name)
{
	unordered_map<string, symItem> s = symbolTable.back();
	unordered_map<string, symItem>::iterator iter = s.find(name);
	if (s.end() != iter)
	{
		return iter->second;
	}
	return{};
}
void popSymTableLevel()
{
	symbolTable.pop_back();
}