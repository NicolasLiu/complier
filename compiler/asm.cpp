#include "global.h"

void gen_asm()
{
	if (errorNum)
	{
		return;
	}
	fout << "include \\masm32\\include\\masm32rt.inc" << endl;

}