#include <iostream>
#include <rawld.h>

void main(int argc, char **argv)
{
	RawLD::createModule("tmp/lextest");
	RawLD lex("tmp/lextest");

	lex.SetKey("b");
	lex << "x";

	lex.SetKey("a");
	lex << "x";

	lex.SetKey("a");
	lex.deleteEntry();

//	lex.SetKey("a");
//	lex << "y";

	lex = BOTTOM;
}
